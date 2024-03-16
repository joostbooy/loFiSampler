import numpy
import sys
dir = '/Users/joostbooy/Desktop/Code/Atom/Python/armScripts'
sys.path.insert(0,dir)
import lutCompiler


#table variables
tables = []
ppqn = 24
max_bpm = 300
clock_isr_freq = 8000
sample_rate = 16000

'''____________________
	BPM INC
________________________'''

name = 'bpm_inc'

bpm = numpy.arange(1.0, max_bpm + 1)
hertz = bpm / 60 * ppqn
values = hertz / clock_isr_freq * (1 << 32);

tables.append('uint32_t ' + name)
tables.append(values.astype(int))

'''____________________
        PHASE INC
________________________'''
name = 'phase_inc'

freq_min = 1.0 / (sample_rate * 10.0)
freq_max = 1.0 / (sample_rate / 10.0)
steps = numpy.linspace(freq_min, freq_max, 256)

tables.append('float ' + name)
tables.append(steps.astype('float32'))

'''_____________________________
     TEMPO SYNC PHASE INC
_____________________________'''

bpm = numpy.arange(1.0, max_bpm + 1)
hertz = bpm / 60 / 4
values = sample_rate * hertz

name = 'tempo_sync_phase_inc'
values_whole = 1.0 / values
tables.append('float ' + name)
tables.append(values_whole.astype('float32'))

'''____________________
	RECIPROCAL
________________________'''
name = 'reciprocal'

max_steps = 16
step = numpy.arange(1, max_steps + 1)
values = 1.0 / step
values = numpy.insert(values, 0, 0.0)

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
	FREQUENCY RATIO
________________________'''
name = 'frequency_ratio'

notes = numpy.arange(128, -128, -1)
values = pow(2, notes / 12.0);

tables.append('float ' + name)
tables.append(values.astype('float32'))


'''____________________
	DEFINES
________________________'''

defines = [
'SAMPLE_RATE '			+ str(sample_rate),
'PPQN '					+ str(ppqn),
'MAX_BPM '				+ str(max_bpm),
'CLOCK_ISR_FREQ '		+ str(clock_isr_freq),
'EXP_TABLE_SIZE '		+ str(exp_table_size)
]


'''____________________
	COMPILING
________________________'''

# set directory to write tables (argument given in makeFile)
lutCompiler.setDir(str(sys.argv[1]))
# compile and store
lutCompiler.compile(tables, defines)
