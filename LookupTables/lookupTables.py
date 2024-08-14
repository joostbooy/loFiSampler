import numpy
import sys
dir = '/Users/joostbooy/Desktop/Code/Atom/Python/armScripts'
sys.path.insert(0,dir)
import lutCompiler


#table variables
tables = []

block_size = 16
sample_rate = 16000
control_rate = sample_rate / block_size

ppqn = 24
min_bpm = 30
max_bpm = 300
clock_isr_freq = 4000


'''____________________
	BPM INC
________________________'''

name = 'bpm_inc'

bpm = numpy.arange(min_bpm, max_bpm + 1)
hertz = bpm / 60 * ppqn
values = hertz / clock_isr_freq * (1 << 32);

tables.append('uint32_t ' + name)
tables.append(values.astype(int))

'''____________________
        PHASE INC
________________________'''
phase_table_size = 256

name = 'phase_length'
freq_min = 0.1
freq_max = 50.0
steps = numpy.linspace(freq_min, freq_max, phase_table_size)

tables.append('float ' + name)
tables.append(steps.astype('float32'))

name = 'phase_inc'
freq_inc_min = freq_min / control_rate
freq_inc_max = freq_max / control_rate
steps = numpy.linspace(freq_inc_min, freq_inc_max, phase_table_size)

tables.append('float ' + name)
tables.append(steps.astype('float32'))

'''_____________________________
     BEAT LENGTH
_____________________________'''

name = 'beat_length'
values = sample_rate * (60 / bpm)
tables.append('float ' + name)
tables.append(values.astype('float32'))

name = 'beat_length_inc'
values = 1.0 / (control_rate * (60 / bpm))
tables.append('float ' + name)
tables.append(values.astype('float32'))

'''____________________
	EXP TABLE
________________________'''

exp_table_size = 1024
stepsize = 1.0 / exp_table_size
x = numpy.arange(0, 1.0, stepsize)

name = 'exp'
values = 1.0 - numpy.exp(-6 * x)
values /= values.max()

tables.append('float ' + name)
tables.append(values.astype('float32'))

name = 'inv_exp'
values = 1.0 - values[::-1]

tables.append('float ' + name)
tables.append(values.astype('float32'))

'''____________________
	FREQUENCY RATIOS
________________________'''
name = 'semitone_ratio'

notes = numpy.arange(-128, 128)
values = pow(2, notes / 12.0);

tables.append('float ' + name)
tables.append(values.astype('float32'))

name = 'cent_ratio'
values = pow(2, numpy.arange(-99, 99) / 198.0 / 12.0);
#values = pow(2, numpy.arange(0, 256) / 256.0 / 12.0)

tables.append('float ' + name)
tables.append(values.astype('float32'))

'''____________________
	DEFINES
________________________'''

defines = [
'BLOCK_SIZE '				+ str(block_size),
'SAMPLE_RATE '				+ str(sample_rate),
'MIN_BPM '					+ str(min_bpm),
'MAX_BPM '					+ str(max_bpm),
'CLOCK_ISR_FREQ '			+ str(clock_isr_freq),
'EXP_TABLE_SIZE '			+ str(exp_table_size),
'PHASE_TABLE_SIZE '			+ str(phase_table_size),
]


'''____________________
	COMPILING
________________________'''

# set directory to write tables (argument given in makeFile)
lutCompiler.setDir(str(sys.argv[1]))
# compile and store
lutCompiler.compile(tables, defines)
