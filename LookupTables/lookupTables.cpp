#include "lookupTables.h"

const uint32_t lut_bpm_inc[300] = {
	214748,429496,644245,858993,1073741,1288490,1503238,
	1717986,1932735,2147483,2362232,2576980,2791728,3006477,3221225,
	3435973,3650722,3865470,4080218,4294967,4509715,4724464,4939212,
	5153960,5368709,5583457,5798205,6012954,6227702,6442450,6657199,
	6871947,7086696,7301444,7516192,7730941,7945689,8160437,8375186,
	8589934,8804682,9019431,9234179,9448928,9663676,9878424,10093173,
	10307921,10522669,10737418,10952166,11166914,11381663,11596411,11811160,
	12025908,12240656,12455405,12670153,12884901,13099650,13314398,13529146,
	13743895,13958643,14173392,14388140,14602888,14817637,15032385,15247133,
	15461882,15676630,15891378,16106127,16320875,16535624,16750372,16965120,
	17179869,17394617,17609365,17824114,18038862,18253611,18468359,18683107,
	18897856,19112604,19327352,19542101,19756849,19971597,20186346,20401094,
	20615843,20830591,21045339,21260088,21474836,21689584,21904333,22119081,
	22333829,22548578,22763326,22978075,23192823,23407571,23622320,23837068,
	24051816,24266565,24481313,24696061,24910810,25125558,25340307,25555055,
	25769803,25984552,26199300,26414048,26628797,26843545,27058293,27273042,
	27487790,27702539,27917287,28132035,28346784,28561532,28776280,28991029,
	29205777,29420525,29635274,29850022,30064771,30279519,30494267,30709016,
	30923764,31138512,31353261,31568009,31782757,31997506,32212254,32427003,
	32641751,32856499,33071248,33285996,33500744,33715493,33930241,34144990,
	34359738,34574486,34789235,35003983,35218731,35433480,35648228,35862976,
	36077725,36292473,36507222,36721970,36936718,37151467,37366215,37580963,
	37795712,38010460,38225208,38439957,38654705,38869454,39084202,39298950,
	39513699,39728447,39943195,40157944,40372692,40587440,40802189,41016937,
	41231686,41446434,41661182,41875931,42090679,42305427,42520176,42734924,
	42949672,43164421,43379169,43593918,43808666,44023414,44238163,44452911,
	44667659,44882408,45097156,45311904,45526653,45741401,45956150,46170898,
	46385646,46600395,46815143,47029891,47244640,47459388,47674136,47888885,
	48103633,48318382,48533130,48747878,48962627,49177375,49392123,49606872,
	49821620,50036368,50251117,50465865,50680614,50895362,51110110,51324859,
	51539607,51754355,51969104,52183852,52398601,52613349,52828097,53042846,
	53257594,53472342,53687091,53901839,54116587,54331336,54546084,54760833,
	54975581,55190329,55405078,55619826,55834574,56049323,56264071,56478819,
	56693568,56908316,57123065,57337813,57552561,57767310,57982058,58196806,
	58411555,58626303,58841051,59055800,59270548,59485297,59700045,59914793,
	60129542,60344290,60559038,60773787,60988535,61203283,61418032,61632780,
	61847529,62062277,62277025,62491774,62706522,62921270,63136019,63350767,
	63565515,63780264,63995012,64209761,64424509,
};

const float lut_phase_inc[256] = {
	6.25e-06,8.67647e-06,1.1102941e-05,1.3529412e-05,1.5955882e-05,1.8382352e-05,2.0808824e-05,
	2.3235294e-05,2.5661764e-05,2.8088236e-05,3.0514706e-05,3.2941178e-05,3.5367648e-05,3.7794118e-05,4.0220588e-05,
	4.2647058e-05,4.5073528e-05,4.75e-05,4.992647e-05,5.235294e-05,5.477941e-05,5.720588e-05,5.963235e-05,
	6.2058825e-05,6.448529e-05,6.6911765e-05,6.933823e-05,7.1764705e-05,7.419118e-05,7.6617645e-05,7.904412e-05,
	8.1470585e-05,8.389706e-05,8.632353e-05,8.875e-05,9.117647e-05,9.360294e-05,9.602941e-05,9.845588e-05,
	0.00010088235,0.00010330883,0.00010573529,0.00010816177,0.00011058823,0.00011301471,0.00011544117,0.00011786765,
	0.00012029412,0.0001227206,0.00012514705,0.00012757353,0.00013,0.00013242647,0.00013485295,0.0001372794,
	0.00013970588,0.00014213235,0.00014455883,0.00014698529,0.00014941176,0.00015183823,0.00015426471,0.00015669118,
	0.00015911764,0.00016154411,0.00016397059,0.00016639706,0.00016882354,0.00017125,0.00017367647,0.00017610294,
	0.00017852942,0.00018095589,0.00018338235,0.00018580882,0.0001882353,0.00019066177,0.00019308823,0.0001955147,
	0.00019794118,0.00020036765,0.00020279412,0.00020522058,0.00020764706,0.00021007353,0.0002125,0.00021492648,
	0.00021735294,0.00021977941,0.00022220588,0.00022463236,0.00022705883,0.00022948529,0.00023191176,0.00023433824,
	0.00023676471,0.00023919117,0.00024161764,0.00024404412,0.0002464706,0.00024889706,0.00025132354,0.00025375,
	0.00025617648,0.00025860293,0.0002610294,0.00026345588,0.00026588235,0.00026830882,0.0002707353,0.00027316177,
	0.00027558825,0.00027801472,0.00028044116,0.00028286764,0.0002852941,0.00028772058,0.00029014706,0.00029257353,
	0.000295,0.00029742648,0.00029985295,0.00030227943,0.00030470587,0.00030713234,0.00030955882,0.0003119853,
	0.00031441177,0.00031683824,0.0003192647,0.0003216912,0.00032411766,0.0003265441,0.00032897058,0.00033139705,
	0.00033382353,0.00033625,0.00033867647,0.00034110295,0.00034352942,0.0003459559,0.00034838234,0.0003508088,
	0.0003532353,0.00035566176,0.00035808823,0.0003605147,0.00036294118,0.00036536765,0.00036779413,0.0003702206,
	0.00037264705,0.00037507352,0.0003775,0.00037992647,0.00038235294,0.0003847794,0.0003872059,0.00038963236,
	0.00039205884,0.00039448528,0.00039691175,0.00039933823,0.0004017647,0.00040419117,0.00040661765,0.00040904412,
	0.0004114706,0.00041389707,0.00041632354,0.00041875,0.00042117646,0.00042360293,0.0004260294,0.00042845588,
	0.00043088236,0.00043330883,0.0004357353,0.00043816178,0.00044058822,0.0004430147,0.00044544117,0.00044786764,
	0.00045029412,0.0004527206,0.00045514706,0.00045757354,0.00046,0.00046242648,0.00046485293,0.0004672794,
	0.00046970588,0.00047213235,0.00047455882,0.0004769853,0.00047941177,0.00048183824,0.00048426472,0.00048669116,
	0.00048911764,0.00049154414,0.0004939706,0.0004963971,0.00049882353,0.00050125,0.0005036765,0.0005061029,
	0.0005085294,0.00051095587,0.0005133824,0.0005158088,0.0005182353,0.00052066176,0.0005230882,0.0005255147,
	0.00052794116,0.00053036766,0.0005327941,0.0005352206,0.00053764705,0.00054007355,0.0005425,0.00054492644,
	0.00054735295,0.0005497794,0.0005522059,0.00055463234,0.00055705884,0.0005594853,0.0005619118,0.00056433823,
	0.00056676473,0.0005691912,0.0005716176,0.0005740441,0.0005764706,0.0005788971,0.0005813235,0.00058375,
	0.00058617647,0.00058860297,0.0005910294,0.00059345586,0.00059588236,0.0005983088,0.0006007353,0.00060316175,
	0.00060558825,0.0006080147,0.0006104412,0.00061286765,0.0006152941,0.0006177206,0.00062014704,0.00062257354,
	0.000625,
};

const float lut_tempo_sync_phase_inc[300] = {
	0.015,0.0075,0.005,0.00375,0.003,0.0025,0.002142857,
	0.001875,0.0016666667,0.0015,0.0013636363,0.00125,0.0011538462,0.0010714285,0.001,
	0.0009375,0.00088235294,0.00083333335,0.0007894737,0.00075,0.0007142857,0.0006818182,0.00065217394,
	0.000625,0.0006,0.0005769231,0.00055555557,0.00053571427,0.00051724137,0.0005,0.00048387097,
	0.00046875,0.00045454546,0.00044117647,0.00042857142,0.00041666668,0.00040540542,0.00039473685,0.00038461538,
	0.000375,0.00036585366,0.00035714285,0.0003488372,0.0003409091,0.00033333333,0.00032608697,0.00031914894,
	0.0003125,0.00030612244,0.0003,0.00029411764,0.00028846154,0.00028301886,0.00027777778,0.00027272728,
	0.00026785713,0.00026315788,0.00025862068,0.0002542373,0.00025,0.00024590164,0.00024193549,0.00023809524,
	0.000234375,0.00023076923,0.00022727273,0.00022388059,0.00022058823,0.00021739131,0.00021428571,0.0002112676,
	0.00020833334,0.00020547945,0.00020270271,0.0002,0.00019736843,0.0001948052,0.00019230769,0.00018987342,
	0.0001875,0.00018518518,0.00018292683,0.0001807229,0.00017857143,0.00017647058,0.0001744186,0.0001724138,
	0.00017045454,0.00016853932,0.00016666666,0.00016483516,0.00016304349,0.00016129033,0.00015957447,0.00015789474,
	0.00015625,0.00015463917,0.00015306122,0.00015151515,0.00015,0.00014851485,0.00014705882,0.00014563107,
	0.00014423077,0.00014285714,0.00014150943,0.00014018692,0.00013888889,0.00013761468,0.00013636364,0.00013513514,
	0.00013392857,0.00013274336,0.00013157894,0.00013043478,0.00012931034,0.00012820512,0.00012711865,0.00012605042,
	0.000125,0.00012396694,0.00012295082,0.00012195122,0.00012096774,0.00012,0.00011904762,0.000118110234,
	0.0001171875,0.00011627907,0.00011538462,0.00011450382,0.000113636364,0.000112781956,0.000111940295,0.00011111111,
	0.00011029412,0.000109489054,0.000108695654,0.00010791367,0.000107142856,0.00010638298,0.0001056338,0.0001048951,
	0.00010416667,0.00010344828,0.00010273972,0.000102040816,0.000101351354,0.00010067114,1e-04,9.933775e-05,
	9.868421e-05,9.8039214e-05,9.74026e-05,9.6774194e-05,9.6153846e-05,9.55414e-05,9.493671e-05,9.433962e-05,
	9.375e-05,9.31677e-05,9.259259e-05,9.202454e-05,9.1463415e-05,9.090909e-05,9.036145e-05,8.982036e-05,
	8.928571e-05,8.87574e-05,8.823529e-05,8.77193e-05,8.72093e-05,8.67052e-05,8.62069e-05,8.571429e-05,
	8.522727e-05,8.474576e-05,8.426966e-05,8.3798885e-05,8.333333e-05,8.287293e-05,8.241758e-05,8.196721e-05,
	8.152174e-05,8.108108e-05,8.0645164e-05,8.0213904e-05,7.9787234e-05,7.9365076e-05,7.894737e-05,7.853403e-05,
	7.8125e-05,7.772021e-05,7.7319586e-05,7.6923076e-05,7.653061e-05,7.614213e-05,7.5757576e-05,7.537688e-05,
	7.5e-05,7.462686e-05,7.425743e-05,7.389163e-05,7.352941e-05,7.317073e-05,7.2815536e-05,7.2463765e-05,
	7.2115385e-05,7.177034e-05,7.142857e-05,7.109005e-05,7.0754715e-05,7.042254e-05,7.009346e-05,6.976744e-05,
	6.9444446e-05,6.912442e-05,6.880734e-05,6.849315e-05,6.818182e-05,6.7873305e-05,6.756757e-05,6.726458e-05,
	6.696428e-05,6.666667e-05,6.637168e-05,6.6079294e-05,6.578947e-05,6.550219e-05,6.521739e-05,6.493506e-05,
	6.465517e-05,6.437768e-05,6.410256e-05,6.3829786e-05,6.3559324e-05,6.329114e-05,6.302521e-05,6.276151e-05,
	6.25e-05,6.2240666e-05,6.198347e-05,6.1728395e-05,6.147541e-05,6.122449e-05,6.097561e-05,6.0728744e-05,
	6.048387e-05,6.0240964e-05,6e-05,5.9760958e-05,5.952381e-05,5.9288537e-05,5.9055117e-05,5.882353e-05,
	5.859375e-05,5.836576e-05,5.8139536e-05,5.791506e-05,5.769231e-05,5.7471265e-05,5.725191e-05,5.703422e-05,
	5.6818182e-05,5.6603774e-05,5.6390978e-05,5.6179775e-05,5.5970148e-05,5.576208e-05,5.5555556e-05,5.5350552e-05,
	5.514706e-05,5.4945056e-05,5.4744527e-05,5.4545453e-05,5.4347827e-05,5.4151624e-05,5.3956835e-05,5.376344e-05,
	5.3571428e-05,5.3380783e-05,5.319149e-05,5.3003532e-05,5.28169e-05,5.2631578e-05,5.244755e-05,5.2264808e-05,
	5.2083335e-05,5.1903113e-05,5.172414e-05,5.154639e-05,5.136986e-05,5.119454e-05,5.1020408e-05,5.084746e-05,
	5.0675677e-05,5.0505052e-05,5.033557e-05,5.0167226e-05,5e-05,
};

const float lut_reciprocal[33] = {
	0.0,1.0,0.5,0.33333334,0.25,0.2,0.16666667,
	0.14285715,0.125,0.11111111,0.1,0.09090909,0.083333336,0.07692308,0.071428575,
	0.06666667,0.0625,0.05882353,0.055555556,0.05263158,0.05,0.04761905,0.045454547,
	0.04347826,0.041666668,0.04,0.03846154,0.037037037,0.035714287,0.03448276,0.033333335,
	0.032258064,0.03125,
};

const float lut_exp[1024] = {
	0.0,0.0058568455,0.011679473,0.017468084,0.023222877,0.028944049,0.034631796,
	0.040286314,0.045907795,0.05149644,0.05705243,0.06257596,0.06806722,0.0735264,0.07895369,
	0.08434927,0.08971332,0.09504604,0.1003476,0.105618194,0.11085799,0.11606718,0.12124593,
	0.12639444,0.13151285,0.13660136,0.14166015,0.14668939,0.15168923,0.15665987,0.16160147,
	0.1665142,0.17139824,0.17625372,0.18108086,0.18587978,0.19065067,0.1953937,0.200109,
	0.20479676,0.20945713,0.21409027,0.21869636,0.22327553,0.22782794,0.23235376,0.23685314,
	0.24132623,0.2457732,0.25019416,0.25458932,0.2589588,0.26330274,0.2676213,0.27191463,
	0.2761829,0.2804262,0.28464475,0.28883862,0.293008,0.29715303,0.30127382,0.30537057,
	0.30944335,0.31349236,0.3175177,0.32151952,0.32549798,0.3294532,0.3333853,0.33729443,
	0.3411807,0.34504429,0.3488853,0.35270387,0.35650015,0.36027423,0.36402628,0.36775637,
	0.3714647,0.37515137,0.3788165,0.38246018,0.38608262,0.38968387,0.39326409,0.39682338,
	0.4003619,0.40387973,0.407377,0.41085386,0.4143104,0.41774675,0.42116302,0.42455932,
	0.42793578,0.43129253,0.43462965,0.4379473,0.44124553,0.44452453,0.44778433,0.45102513,
	0.45424697,0.45745,0.46063432,0.4638,0.46694723,0.47007605,0.4731866,0.476279,
	0.47935328,0.48240963,0.48544812,0.48846886,0.49147195,0.4944575,0.49742562,0.5003764,
	0.5033099,0.5062263,0.50912565,0.5120081,0.5148736,0.5177224,0.5205546,0.52337027,
	0.5261695,0.5289523,0.53171885,0.5344693,0.5372036,0.539922,0.5426245,0.5453112,
	0.5479822,0.5506376,0.5532775,0.55590194,0.5585111,0.56110495,0.5636837,0.5662474,
	0.56879604,0.5713299,0.57384884,0.57635313,0.5788428,0.5813179,0.58377856,0.5862248,
	0.5886568,0.5910746,0.5934782,0.5958678,0.5982435,0.6006052,0.6029532,0.60528743,
	0.607608,0.6099151,0.61220866,0.61448884,0.61675566,0.6190093,0.62124974,0.6234771,
	0.6256914,0.62789285,0.63008136,0.63225716,0.63442016,0.6365706,0.6387084,0.6408338,
	0.6429467,0.6450473,0.6471357,0.64921176,0.65127575,0.6533277,0.6553677,0.65739566,
	0.65941185,0.66141623,0.66340894,0.66539,0.6673595,0.6693174,0.671264,0.6731991,
	0.675123,0.67703557,0.678937,0.6808274,0.68270665,0.68457496,0.68643236,0.6882789,
	0.6901147,0.69193965,0.693754,0.69555783,0.69735104,0.6991338,0.7009061,0.70266813,
	0.7044198,0.70616126,0.70789254,0.7096137,0.7113248,0.7130259,0.7147171,0.7163984,
	0.71806985,0.71973157,0.7213836,0.7230259,0.72465867,0.7262819,0.7278956,0.72949994,
	0.73109484,0.73268044,0.7342568,0.7358239,0.73738194,0.7389308,0.74047065,0.7420015,
	0.74352336,0.74503636,0.74654055,0.7480359,0.74952257,0.7510005,0.75246984,0.75393057,
	0.7553828,0.75682646,0.75826174,0.7596887,0.7611072,0.76251745,0.76391953,0.7653133,
	0.7666991,0.76807666,0.7694462,0.77080774,0.77216136,0.77350706,0.7748449,0.7761749,
	0.7774972,0.7788117,0.7801185,0.7814177,0.7827093,0.78399336,0.78527,0.7865391,
	0.7878007,0.7890551,0.7903021,0.79154176,0.79277426,0.7939995,0.79521763,0.7964286,
	0.7976325,0.79882944,0.8000193,0.80120224,0.8023783,0.80354744,0.80470973,0.8058653,
	0.80701405,0.80815613,0.80929154,0.81042033,0.8115425,0.81265813,0.8137672,0.8148698,
	0.815966,0.81705576,0.8181392,0.81921625,0.82028705,0.8213516,0.82240987,0.823462,
	0.824508,0.8255479,0.82658166,0.8276094,0.82863116,0.82964694,0.8306568,0.83166075,
	0.8326588,0.83365107,0.8346375,0.8356182,0.8365932,0.83756244,0.83852607,0.83948404,
	0.8404364,0.8413832,0.8423245,0.8432603,0.8441906,0.8451155,0.84603494,0.84694904,
	0.84785783,0.84876126,0.84965944,0.8505524,0.8514401,0.85232264,0.85319996,0.8540722,
	0.8549394,0.85580146,0.8566585,0.85751057,0.8583576,0.8591997,0.8600369,0.8608692,
	0.86169666,0.86251926,0.86333704,0.86415005,0.86495835,0.8657619,0.86656076,0.8673549,
	0.86814445,0.8689294,0.8697098,0.87048554,0.8712568,0.8720236,0.8727858,0.8735437,
	0.874297,0.875046,0.87579066,0.8765309,0.8772668,0.8779985,0.8787259,0.87944895,
	0.88016784,0.88088256,0.88159305,0.8822994,0.8830017,0.88369983,0.8843939,0.88508385,
	0.88576984,0.8864518,0.8871298,0.8878038,0.88847387,0.88914007,0.88980234,0.8904607,
	0.8911153,0.891766,0.89241296,0.8930561,0.8936955,0.89433116,0.8949631,0.8955914,
	0.8962159,0.8968369,0.8974542,0.8980679,0.898678,0.89928454,0.89988756,0.90048707,
	0.901083,0.9016755,0.90226454,0.90285015,0.9034323,0.90401113,0.9045865,0.9051585,
	0.9057272,0.9062926,0.9068546,0.9074134,0.90796894,0.90852123,0.90907025,0.9096161,
	0.91015875,0.91069824,0.91123456,0.9117677,0.91229784,0.9128248,0.91334873,0.91386956,
	0.91438735,0.9149021,0.91541386,0.91592264,0.91642845,0.91693133,0.91743124,0.9179282,
	0.9184223,0.9189135,0.9194018,0.9198873,0.9203699,0.92084974,0.92132676,0.92180103,
	0.92227244,0.9227412,0.92320716,0.9236704,0.9241309,0.9245888,0.92504394,0.92549646,
	0.92594635,0.92639357,0.9268382,0.92728025,0.9277197,0.92815655,0.9285909,0.92902267,
	0.92945194,0.9298787,0.930303,0.9307248,0.9311441,0.931561,0.9319754,0.9323875,
	0.9327971,0.9332043,0.9336091,0.9340116,0.9344117,0.9348095,0.935205,0.93559813,
	0.93598896,0.9363775,0.9367638,0.93714786,0.9375297,0.93790925,0.9382866,0.93866175,
	0.9390347,0.9394055,0.9397741,0.94014055,0.94050485,0.94086707,0.94122714,0.9415851,
	0.94194096,0.9422948,0.9426465,0.9429962,0.9433438,0.9436894,0.944033,0.94437456,
	0.9447141,0.9450517,0.94538736,0.94572103,0.94605273,0.9463825,0.94671035,0.9470363,
	0.94736034,0.9476825,0.94800276,0.9483211,0.94863766,0.9489523,0.9492651,0.94957614,
	0.94988537,0.95019275,0.95049834,0.95080215,0.95110416,0.9514044,0.95170295,0.9519997,
	0.9522947,0.952588,0.9528796,0.9531695,0.9534577,0.95374423,0.9540291,0.95431226,
	0.9545938,0.9548737,0.9551519,0.9554285,0.9557035,0.9559769,0.9562487,0.9565189,
	0.9567875,0.9570546,0.9573201,0.957584,0.95784646,0.95810735,0.9583667,0.95862454,
	0.95888084,0.95913565,0.95938903,0.95964086,0.95989126,0.9601402,0.96038765,0.9606337,
	0.9608783,0.96112144,0.9613632,0.9616035,0.9618424,0.96208,0.9623161,0.9625509,
	0.9627843,0.9630163,0.96324694,0.9634763,0.9637043,0.9639309,0.96415627,0.96438026,
	0.96460295,0.9648244,0.9650445,0.9652633,0.96548086,0.9656971,0.9659121,0.96612585,
	0.9663384,0.96654963,0.9667597,0.9669685,0.967176,0.96738243,0.9675876,0.96779156,
	0.96799433,0.9681959,0.9683963,0.96859556,0.96879363,0.96899056,0.9691863,0.9693809,
	0.96957445,0.9697668,0.969958,0.97014815,0.97033715,0.970525,0.9707118,0.97089756,
	0.97108215,0.97126573,0.9714482,0.9716296,0.97181,0.9719893,0.9721675,0.9723447,
	0.9725209,0.972696,0.9728702,0.97304326,0.97321534,0.9733864,0.9735565,0.9737256,
	0.9738937,0.97406083,0.97422695,0.9743922,0.9745564,0.9747196,0.9748819,0.97504324,
	0.97520363,0.97536314,0.9755217,0.9756793,0.975836,0.9759917,0.97614664,0.9763006,
	0.97645366,0.9766058,0.97675705,0.9769075,0.977057,0.97720563,0.9773534,0.9775003,
	0.97764635,0.97779155,0.9779359,0.97807944,0.9782221,0.97836393,0.97850496,0.9786451,
	0.9787845,0.978923,0.97906077,0.9791977,0.9793338,0.9794692,0.9796037,0.97973746,
	0.97987044,0.98000264,0.9801341,0.9802648,0.98039466,0.9805238,0.9806522,0.9807798,
	0.9809067,0.98103285,0.9811583,0.98128295,0.9814069,0.98153013,0.9816527,0.98177445,
	0.9818955,0.9820159,0.9821356,0.98225456,0.9823728,0.9824904,0.9826073,0.98272353,
	0.98283905,0.9829539,0.9830681,0.98318166,0.9832945,0.9834067,0.98351824,0.9836291,
	0.9837394,0.983849,0.98395795,0.98406625,0.98417395,0.984281,0.98438746,0.98449326,
	0.98459846,0.98470306,0.984807,0.98491037,0.9850131,0.9851153,0.98521686,0.9853178,
	0.9854182,0.98551804,0.9856172,0.98571587,0.9858139,0.98591137,0.9860083,0.98610467,
	0.98620045,0.98629564,0.9863903,0.9864844,0.986578,0.98667103,0.9867635,0.98685545,
	0.9869468,0.98703766,0.987128,0.98721784,0.9873071,0.9873959,0.9874841,0.98757184,
	0.98765904,0.98774576,0.98783195,0.9879176,0.98800284,0.98808753,0.9881717,0.98825544,
	0.98833865,0.9884214,0.98850363,0.98858535,0.98866665,0.9887475,0.9888278,0.9889077,
	0.9889871,0.98906606,0.9891445,0.9892225,0.98930013,0.98937726,0.9894539,0.9895301,
	0.9896059,0.9896812,0.9897561,0.98983055,0.9899046,0.98997813,0.9900513,0.99012405,
	0.99019635,0.99026823,0.9903397,0.9904107,0.9904813,0.99055153,0.9906213,0.99069077,
	0.99075973,0.99082834,0.9908965,0.9909643,0.9910317,0.9910987,0.9911653,0.9912315,
	0.99129736,0.9913628,0.99142784,0.9914925,0.9915568,0.9916208,0.9916843,0.9917475,
	0.9918103,0.9918728,0.99193484,0.9919966,0.9920579,0.99211895,0.9921796,0.9922399,
	0.9922998,0.9923594,0.99241865,0.99247754,0.9925361,0.9925943,0.9926522,0.9927097,
	0.9927669,0.9928238,0.9928803,0.9929365,0.99299234,0.9930479,0.99310315,0.99315804,
	0.9932126,0.9932668,0.99332076,0.9933744,0.99342775,0.99348074,0.99353343,0.9935858,
	0.99363786,0.99368966,0.9937411,0.9937923,0.99384314,0.99389374,0.993944,0.993994,
	0.9940437,0.99409306,0.9941422,0.994191,0.99423957,0.9942878,0.9943358,0.9943835,
	0.9944309,0.99447805,0.9945249,0.9945715,0.9946178,0.99466383,0.9947096,0.99475515,
	0.9948004,0.9948454,0.9948901,0.99493456,0.9949787,0.99502265,0.99506634,0.9951098,
	0.99515295,0.99519587,0.99523854,0.995281,0.9953231,0.9953651,0.99540675,0.9954482,
	0.99548936,0.99553037,0.9955711,0.99561155,0.9956518,0.9956918,0.99573153,0.9957711,
	0.9958104,0.9958495,0.99588835,0.995927,0.99596536,0.9960035,0.9960415,0.9960792,
	0.9961167,0.996154,0.9961911,0.9962279,0.9962646,0.996301,0.99633723,0.99637324,
	0.996409,0.9964446,0.99648,0.99651515,0.9965501,0.99658483,0.9966194,0.9966538,
	0.99668795,0.99672186,0.9967556,0.99678916,0.99682254,0.99685574,0.9968887,0.9969215,
	0.9969541,0.99698645,0.99701864,0.9970507,0.99708253,0.9971142,0.99714565,0.9971769,
	0.997208,0.99723893,0.99726963,0.9973002,0.9973306,0.99736077,0.9973908,0.99742067,
	0.9974503,0.9974798,0.9975091,0.9975383,0.9975673,0.9975961,0.99762475,0.99765325,
	0.99768156,0.9977097,0.99773765,0.9977655,0.99779314,0.9978206,0.997848,0.99787515,
	0.99790215,0.99792904,0.99795574,0.99798226,0.99800867,0.9980349,0.998061,0.9980869,
	0.9981127,0.9981383,0.9981638,0.99818915,0.9982143,0.99823934,0.99826425,0.998289,
	0.9983136,0.99833804,0.99836236,0.9983865,0.9984106,0.9984344,0.9984582,0.9984818,
	0.9985053,0.9985286,0.9985518,0.99857485,0.9985978,0.9986206,0.9986433,0.9986658,
	0.99868816,0.99871045,0.99873257,0.9987546,0.9987765,0.99879825,0.9988198,0.99884135,
	0.99886274,0.99888396,0.9989051,0.9989261,0.99894696,0.9989677,0.9989884,0.9990089,
	0.9990293,0.99904954,0.9990697,0.9990897,0.9991097,0.9991295,0.99914914,0.99916875,
	0.9991882,0.99920756,0.99922675,0.9992459,0.9992649,0.9992838,0.99930257,0.9993213,
	0.9993398,0.9993583,0.99937665,0.9993949,0.999413,0.9994311,0.99944896,0.9994668,
	0.99948454,0.9995021,0.99951965,0.99953705,0.99955434,0.99957156,0.99958867,0.99960566,
	0.9996226,0.9996394,0.9996561,0.9996727,0.9996892,0.9997057,0.99972194,0.9997382,
	0.9997543,0.99977034,0.9997863,0.9998022,0.9998179,0.9998336,0.99984914,0.99986464,
	0.99988,0.99989533,0.99991053,0.9999257,0.99994075,0.99995565,0.99997056,0.99998534,
	1.0,
};

const float lut_inv_exp[1024] = {
	0.0,1.4688798e-05,2.9463916e-05,4.432586e-05,5.9275142e-05,7.431227e-05,8.943777e-05,
	0.00010465216,0.000119955956,0.00013534968,0.00015083388,0.00016640905,0.00018207578,0.00019783455,0.00021368594,
	0.00022963047,0.00024566872,0.0002618012,0.00027802848,0.00029435114,0.0003107697,0.00032728477,0.00034389689,
	0.00036060662,0.00037741454,0.00039432125,0.00041132732,0.0004284333,0.00044563983,0.00046294747,0.0004803568,
	0.00049786846,0.000515483,0.0005332011,0.0005510233,0.0005689502,0.00058698247,0.00060512073,0.0006233656,
	0.00064171763,0.0006601775,0.0006787459,0.0006974234,0.00071621063,0.00073510833,0.00075411703,0.00077323744,
	0.00079247024,0.000811816,0.00083127554,0.0008508494,0.0008705383,0.00089034287,0.0009102638,0.00093030185,
	0.00095045764,0.00097073184,0.0009911252,0.0010116384,0.0010322722,0.0010530272,0.0010739042,0.0010949039,
	0.001116027,0.0011372742,0.0011586463,0.0011801439,0.0012017679,0.001223519,0.001245398,0.0012674055,
	0.0012895422,0.001311809,0.0013342068,0.0013567362,0.0013793979,0.0014021929,0.0014251218,0.0014481854,
	0.0014713845,0.0014947201,0.0015181927,0.0015418032,0.0015655525,0.0015894414,0.0016134707,0.0016376412,
	0.0016619537,0.0016864091,0.0017110083,0.0017357519,0.001760641,0.0017856763,0.0018108588,0.0018361892,
	0.0018616685,0.0018872976,0.0019130772,0.0019390084,0.0019650918,0.0019913288,0.0020177197,0.0020442659,
	0.0020709678,0.002097827,0.0021248437,0.0021520192,0.0021793547,0.0022068506,0.002234508,0.002262328,
	0.0022903115,0.0023184596,0.002346773,0.0023752528,0.0024039,0.0024327154,0.0024617002,0.0024908555,
	0.002520182,0.0025496806,0.0025793528,0.0026091994,0.0026392215,0.0026694199,0.0026997956,0.00273035,
	0.002761084,0.0027919984,0.0028230946,0.0028543735,0.0028858362,0.002917484,0.0029493175,0.0029813382,
	0.003013547,0.0030459452,0.0030785338,0.0031113138,0.0031442863,0.0031774528,0.0032108142,0.0032443716,
	0.0032781262,0.003312079,0.0033462315,0.0033805848,0.0034151399,0.003449898,0.0034848603,0.0035200282,
	0.0035554026,0.003590985,0.0036267766,0.0036627783,0.0036989918,0.003735418,0.0037720583,0.0038089138,
	0.003845986,0.003883276,0.003920785,0.003958515,0.003996466,0.0040346403,0.004073039,0.0041116634,
	0.0041505145,0.004189594,0.0042289034,0.0042684437,0.0043082163,0.0043482226,0.004388464,0.004428942,
	0.004469658,0.004510613,0.004551809,0.0045932466,0.004634928,0.0046768542,0.004719027,0.0047614477,
	0.004804117,0.0048470376,0.0048902105,0.004933637,0.004977319,0.0050212573,0.0050654537,0.00510991,
	0.0051546274,0.005199608,0.0052448525,0.005290363,0.005336141,0.005382188,0.005428506,0.0054750955,
	0.005521959,0.0055690985,0.0056165145,0.005664209,0.005712184,0.0057604406,0.005808981,0.005857807,
	0.0059069195,0.005956321,0.006006012,0.006055996,0.0061062733,0.006156846,0.006207716,0.006258885,
	0.0063103545,0.0063621267,0.006414203,0.006466585,0.0065192753,0.0065722754,0.0066255867,0.0066792113,
	0.0067331507,0.006787407,0.0068419827,0.006896879,0.006952098,0.0070076413,0.007063511,0.007119709,
	0.007176237,0.0072330977,0.0072902925,0.007347823,0.0074056922,0.007463901,0.007522452,0.007581347,
	0.007640588,0.0077001774,0.007760117,0.007820409,0.007881055,0.007942057,0.008003418,0.008065139,
	0.008127224,0.008189673,0.008252488,0.008315674,0.00837923,0.00844316,0.008507466,0.008572149,
	0.008637213,0.008702659,0.00876849,0.008834708,0.008901314,0.008968312,0.009035704,0.009103491,
	0.009171678,0.009240264,0.009309255,0.009378649,0.009448453,0.009518666,0.009589292,0.009660333,
	0.009731792,0.0098036695,0.009875971,0.009948696,0.01002185,0.010095432,0.010169447,0.010243898,
	0.010318786,0.010394114,0.010469885,0.0105461,0.010622764,0.010699878,0.0107774455,0.010855469,
	0.010933951,0.011012894,0.0110923005,0.0111721745,0.011252517,0.011333332,0.011414622,0.01149639,
	0.011578638,0.011661369,0.011744587,0.011828294,0.011912493,0.011997186,0.012082377,0.012168069,
	0.012254264,0.012340967,0.0124281775,0.012515902,0.012604141,0.0126928985,0.012782179,0.0128719825,
	0.012962314,0.013053178,0.013144574,0.013236508,0.0133289825,0.013422,0.013515564,0.0136096785,
	0.013704346,0.013799569,0.013895351,0.013991698,0.014088609,0.014186091,0.014284145,0.014382776,
	0.014481986,0.01458178,0.01468216,0.014783129,0.014884692,0.014986851,0.0150896115,0.015192976,
	0.015296947,0.01540153,0.015506727,0.015612542,0.01571898,0.01582604,0.015933733,0.016042057,
	0.016151018,0.01626062,0.016370866,0.016481759,0.016593305,0.016705506,0.016818365,0.016931888,
	0.017046079,0.01716094,0.017276477,0.017392691,0.01750959,0.017627176,0.017745452,0.017864423,
	0.017984092,0.018104468,0.018225547,0.01834734,0.018469848,0.018593077,0.01871703,0.01884171,
	0.018967124,0.019093273,0.019220166,0.019347804,0.01947619,0.019605333,0.019735234,0.019865898,
	0.01999733,0.020129535,0.020262517,0.02039628,0.02053083,0.020666169,0.020802304,0.020939238,
	0.02107698,0.021215528,0.021354891,0.021495074,0.02163608,0.021777915,0.021920582,0.02206409,
	0.02220844,0.022353638,0.02249969,0.0226466,0.022794373,0.022943014,0.02309253,0.023242924,
	0.023394201,0.023546368,0.023699429,0.02385339,0.024008254,0.024164028,0.02432072,0.024478331,
	0.024636868,0.024796337,0.024956744,0.025118092,0.02528039,0.025443641,0.02560785,0.025773026,
	0.025939172,0.026106294,0.0262744,0.02644349,0.026613576,0.026784662,0.026956754,0.027129855,
	0.027303975,0.027479118,0.02765529,0.027832497,0.028010745,0.028190041,0.028370392,0.0285518,
	0.028734276,0.028917825,0.02910245,0.029288162,0.029474964,0.029662866,0.02985187,0.030041985,
	0.03023322,0.030425575,0.030619062,0.030813687,0.031009454,0.031206373,0.031404447,0.031603687,
	0.031804096,0.032005686,0.032208458,0.03241242,0.032617584,0.032823954,0.033031534,0.033240337,
	0.033450365,0.033661626,0.03387413,0.03408788,0.03430289,0.034519162,0.034736708,0.034955528,
	0.035175636,0.035397038,0.03561974,0.03584375,0.036069077,0.03629573,0.03652371,0.036753036,
	0.036983706,0.037215732,0.03744912,0.03768388,0.037920024,0.038157552,0.038396478,0.038636807,
	0.03887855,0.03912171,0.039366297,0.039612327,0.0398598,0.04010873,0.04035912,0.04061098,
	0.040864322,0.041119155,0.041375484,0.04163332,0.04189267,0.04215354,0.04241595,0.0426799,
	0.0429454,0.04321246,0.04348109,0.0437513,0.044023097,0.04429649,0.04457149,0.044848107,
	0.04512635,0.045406226,0.045687746,0.045970924,0.046255764,0.04654228,0.046830475,0.047120366,
	0.047411963,0.04770527,0.048000302,0.04829707,0.04859558,0.048895843,0.049197875,0.049501676,
	0.049807265,0.05011465,0.050423842,0.05073485,0.051047686,0.05136236,0.051678885,0.051997267,
	0.052317522,0.05263966,0.05296369,0.053289622,0.053617474,0.053947248,0.054278962,0.054612625,
	0.054948248,0.055285845,0.055625428,0.055967003,0.056310583,0.056656186,0.05700382,0.057353497,
	0.057705227,0.058059026,0.058414903,0.05877287,0.05913294,0.05949513,0.059859447,0.060225904,
	0.060594514,0.060965292,0.061338246,0.061713394,0.062090747,0.062470317,0.062852114,0.06323616,
	0.06362246,0.06401103,0.06440189,0.06479504,0.0651905,0.06558829,0.06598841,0.06639089,
	0.06679572,0.06720294,0.06761255,0.068024576,0.068439014,0.06885589,0.069275215,0.06969701,
	0.07012127,0.070548035,0.07097731,0.0714091,0.07184342,0.07228031,0.07271976,0.07316179,
	0.07360642,0.07405366,0.07450353,0.074956045,0.07541122,0.07586907,0.07632961,0.07679285,
	0.07725882,0.07772753,0.078198984,0.07867322,0.07915024,0.079630055,0.080112696,0.080598176,
	0.0810865,0.0815777,0.08207179,0.08256878,0.08306869,0.08357154,0.08407734,0.08458612,
	0.08509789,0.085612655,0.086130455,0.086651295,0.0871752,0.08770218,0.08823226,0.08876545,
	0.08930177,0.08984125,0.090383895,0.09092973,0.09147878,0.09203105,0.09258656,0.09314535,
	0.09370741,0.09427278,0.094841465,0.0954135,0.09598889,0.09656767,0.09714984,0.09773544,
	0.09832448,0.09891698,0.09951296,0.100112446,0.10071545,0.101322,0.101932116,0.10254582,
	0.10316312,0.10378406,0.104408644,0.1050369,0.10566884,0.106304504,0.1069439,0.10758705,
	0.10823398,0.108884715,0.10953927,0.11019768,0.11085995,0.11152612,0.1121962,0.11287022,
	0.1135482,0.114230156,0.11491613,0.11560613,0.11630019,0.11699832,0.11770056,0.11840693,
	0.119117446,0.119832136,0.12055103,0.12127414,0.12200151,0.122733146,0.123469085,0.12420935,
	0.12495396,0.12570295,0.12645635,0.12721416,0.12797643,0.12874319,0.12951444,0.13029024,
	0.13107058,0.13185552,0.13264507,0.13343926,0.13423811,0.13504165,0.13584994,0.13666295,
	0.13748075,0.13830335,0.1391308,0.13996309,0.14080028,0.1416424,0.14248946,0.1433415,
	0.14419855,0.14506063,0.14592777,0.14680003,0.14767739,0.14855991,0.14944763,0.15034056,
	0.15123874,0.15214218,0.15305096,0.15396506,0.15488453,0.15580942,0.15673973,0.1576755,
	0.1586168,0.1595636,0.16051598,0.16147394,0.16243756,0.16340682,0.16438177,0.16536246,
	0.16634892,0.16734117,0.16833925,0.1693432,0.17035306,0.17136884,0.17239058,0.17341834,
	0.17445213,0.175492,0.17653798,0.1775901,0.17864841,0.17971295,0.18078373,0.1818608,
	0.18294422,0.18403399,0.18513018,0.18623279,0.1873419,0.1884575,0.18957968,0.19070844,
	0.19184385,0.19298592,0.19413471,0.19529025,0.19645257,0.19762173,0.19879776,0.19998069,
	0.2011706,0.20236747,0.20357138,0.20478238,0.20600048,0.20722575,0.20845821,0.20969793,
	0.21094492,0.21219924,0.21346094,0.21473004,0.2160066,0.21729068,0.21858229,0.21988149,
	0.22118834,0.22250286,0.2238251,0.22515512,0.22649294,0.22783864,0.22919223,0.23055379,
	0.23192334,0.23330094,0.23468664,0.23608048,0.23748252,0.2388928,0.24031135,0.24173824,
	0.24317352,0.24461724,0.24606945,0.24753018,0.2489995,0.25047746,0.2519641,0.25345945,
	0.25496364,0.25647664,0.25799853,0.25952935,0.26106918,0.26261806,0.26417604,0.2657432,
	0.26731953,0.26890516,0.2705001,0.27210438,0.27371812,0.27534133,0.27697408,0.27861643,
	0.28026843,0.28193015,0.2836016,0.2852829,0.28697407,0.2886752,0.29038632,0.29210746,
	0.29383874,0.2955802,0.2973319,0.29909387,0.30086622,0.30264896,0.3044422,0.30624595,
	0.30806032,0.30988535,0.31172112,0.31356764,0.31542504,0.31729335,0.31917262,0.32106295,
	0.3229644,0.32487702,0.32680085,0.32873604,0.33068255,0.33264053,0.33461002,0.33659106,
	0.33858374,0.34058815,0.34260434,0.34463236,0.3466723,0.34872425,0.35078824,0.35286435,
	0.35495266,0.35705328,0.3591662,0.36129156,0.3634294,0.3655798,0.36774287,0.3699186,
	0.37210715,0.37430856,0.3765229,0.37875026,0.3809907,0.3832443,0.38551116,0.38779134,
	0.39008492,0.39239198,0.39471257,0.39704683,0.39939478,0.40175655,0.4041322,0.4065218,
	0.40892544,0.41134322,0.41377518,0.41622144,0.4186821,0.4211572,0.42364687,0.42615113,
	0.42867014,0.43120393,0.43375263,0.43631628,0.43889502,0.44148892,0.44409806,0.4467225,
	0.4493624,0.45201778,0.45468882,0.4573755,0.460078,0.46279636,0.46553072,0.46828112,
	0.4710477,0.47383055,0.47662973,0.47944537,0.48227754,0.48512638,0.48799196,0.49087435,
	0.4937737,0.4966901,0.49962363,0.5025744,0.5055425,0.50852805,0.5115311,0.5145519,
	0.51759034,0.5206467,0.52372104,0.5268134,0.5299239,0.53305274,0.5362,0.5393657,
	0.54255,0.545753,0.5489749,0.55221564,0.5554755,0.55875444,0.5620527,0.5653703,
	0.56870747,0.5720642,0.5754407,0.578837,0.5822533,0.5856896,0.58914614,0.592623,
	0.5961203,0.5996381,0.6031766,0.6067359,0.61031616,0.6139174,0.6175398,0.6211835,
	0.62484866,0.6285353,0.63224363,0.63597375,0.63972574,0.64349985,0.64729613,0.6511147,
	0.6549557,0.6588193,0.6627056,0.6666147,0.6705468,0.674502,0.67848045,0.6824823,
	0.68650764,0.69055665,0.69462943,0.6987262,0.702847,0.706992,0.7111614,0.7153553,
	0.7195738,0.7238171,0.72808534,0.7323787,0.73669726,0.7410412,0.7454107,0.7498058,
	0.7542268,0.7586738,0.7631469,0.76764625,0.77217203,0.77672446,0.78130364,0.7859097,
	0.79054284,0.79520327,0.799891,0.8046063,0.8093493,0.81412023,0.8189191,0.82374626,
	0.8286018,0.8334858,0.8383985,0.8433401,0.84831077,0.85331064,0.85833985,0.8633986,
	0.8684872,0.87360555,0.8787541,0.8839328,0.88914204,0.8943818,0.8996524,0.90495396,
	0.91028666,0.9156507,0.9210463,0.9264736,0.9319328,0.93742406,0.94294757,0.94850355,
	0.9540922,0.9597137,0.9653682,0.9710559,0.97677714,0.9825319,0.9883205,0.9941431,
	1.0,
};

const float lut_semitone_ratio[256] = {
	0.00061519584,0.00065177726,0.00069053395,0.00073159527,0.00077509816,0.0008211879,0.00087001827,
	0.00092175225,0.0009765625,0.001034632,0.0010961543,0.001161335,0.0012303917,0.0013035545,0.0013810679,
	0.0014631905,0.0015501963,0.0016423758,0.0017400365,0.0018435045,0.001953125,0.002069264,0.0021923087,
	0.00232267,0.0024607833,0.002607109,0.0027621358,0.002926381,0.0031003926,0.0032847517,0.003480073,
	0.003687009,0.00390625,0.004138528,0.0043846173,0.00464534,0.0049215667,0.005214218,0.0055242716,
	0.005852762,0.0062007853,0.0065695033,0.006960146,0.007374018,0.0078125,0.008277056,0.008769235,
	0.00929068,0.009843133,0.010428436,0.011048543,0.011705524,0.012401571,0.013139007,0.013920292,
	0.014748036,0.015625,0.016554112,0.01753847,0.01858136,0.019686267,0.020856872,0.022097087,
	0.023411049,0.024803141,0.026278013,0.027840585,0.029496072,0.03125,0.033108223,0.03507694,
	0.03716272,0.039372534,0.041713744,0.044194173,0.046822097,0.049606282,0.052556027,0.05568117,
	0.058992144,0.0625,0.06621645,0.07015388,0.07432544,0.07874507,0.08342749,0.088388346,
	0.093644194,0.099212565,0.10511205,0.11136234,0.11798429,0.125,0.1324329,0.14030775,
	0.14865088,0.15749013,0.16685498,0.17677669,0.18728839,0.19842513,0.2102241,0.22272468,
	0.23596857,0.25,0.2648658,0.2806155,0.29730177,0.31498027,0.33370996,0.35355338,
	0.37457678,0.39685026,0.4204482,0.44544935,0.47193715,0.5,0.5297316,0.561231,
	0.59460354,0.62996054,0.6674199,0.70710677,0.74915355,0.7937005,0.8408964,0.8908987,
	0.9438743,1.0,1.0594631,1.122462,1.1892071,1.2599211,1.3348398,1.4142135,
	1.4983071,1.587401,1.6817929,1.7817974,1.8877486,2.0,2.1189263,2.244924,
	2.3784142,2.5198421,2.6696796,2.828427,2.9966142,3.174802,3.3635857,3.5635948,
	3.7754972,4.0,4.2378526,4.489848,4.7568283,5.0396843,5.3393593,5.656854,
	5.9932284,6.349604,6.7271714,7.1271896,7.5509944,8.0,8.475705,8.979696,
	9.513657,10.079369,10.678719,11.313708,11.986457,12.699208,13.454343,14.254379,
	15.101989,16.0,16.95141,17.959393,19.027313,20.158737,21.357437,22.627417,
	23.972914,25.398417,26.908686,28.508759,30.203978,32.0,33.90282,35.918785,
	38.054626,40.317474,42.714874,45.254833,47.945827,50.796833,53.81737,57.017517,
	60.407955,64.0,67.80564,71.83757,76.10925,80.63495,85.42975,90.50967,
	95.891655,101.593666,107.63474,114.035034,120.81591,128.0,135.61128,143.67514,
	152.2185,161.2699,170.8595,181.01933,191.78331,203.18733,215.26949,228.07007,
	241.63182,256.0,271.22256,287.35028,304.437,322.5398,341.719,362.03867,
	383.56662,406.37466,430.53897,456.14014,483.26364,512.0,542.4451,574.70056,
	608.874,645.0796,683.438,724.07733,767.13324,812.7493,861.07794,912.2803,
	966.5273,1024.0,1084.8903,1149.4011,1217.748,1290.1592,1366.876,1448.1547,
	1534.2665,
};

const float lut_cent_ratio[198] = {
	0.97167295,0.97195506,0.97223717,0.97251946,0.97280174,0.97308415,0.9733667,
	0.97364926,0.9739319,0.9742146,0.97449744,0.9747804,0.9750633,0.9753464,0.97562957,
	0.9759128,0.9761961,0.9764795,0.97676295,0.97704655,0.97733015,0.97761387,0.9778977,
	0.9781816,0.97846556,0.97874963,0.97903377,0.97931796,0.9796023,0.97988665,0.9801711,
	0.98045564,0.9807403,0.981025,0.9813098,0.9815947,0.98187965,0.9821647,0.9824498,
	0.98273504,0.9830203,0.9833057,0.98359114,0.9838767,0.98416233,0.984448,0.9847338,
	0.9850197,0.9853056,0.98559165,0.9858778,0.986164,0.9864503,0.98673666,0.9870231,
	0.98730963,0.9875963,0.987883,0.9881698,0.9884566,0.9887436,0.9890306,0.9893178,
	0.98960495,0.98989224,0.9901796,0.9904671,0.9907546,0.9910422,0.9913299,0.99161774,
	0.99190557,0.9921935,0.9924816,0.9927697,0.9930579,0.9933462,0.9936346,0.993923,
	0.99421155,0.99450016,0.9947889,0.99507767,0.9953666,0.99565554,0.99594456,0.9962337,
	0.9965229,0.9968122,0.9971016,0.99739105,0.9976806,0.9979702,0.99825996,0.99854976,
	0.9988396,0.9991296,0.9994196,0.9997098,1.0,1.0002903,1.0005807,1.0008712,
	1.0011617,1.0014523,1.0017431,1.002034,1.0023248,1.0026158,1.0029068,1.003198,
	1.0034893,1.0037805,1.004072,1.0043634,1.004655,1.0049467,1.0052384,1.0055302,
	1.0058222,1.0061141,1.0064062,1.0066984,1.0069907,1.007283,1.0075754,1.0078679,
	1.0081605,1.0084531,1.0087459,1.0090388,1.0093317,1.0096247,1.0099177,1.010211,
	1.0105042,1.0107976,1.011091,1.0113846,1.0116782,1.0119718,1.0122657,1.0125595,
	1.0128535,1.0131475,1.0134416,1.0137358,1.0140301,1.0143245,1.014619,1.0149136,
	1.0152081,1.0155028,1.0157976,1.0160925,1.0163876,1.0166826,1.0169778,1.017273,
	1.0175683,1.0178638,1.0181592,1.0184548,1.0187504,1.0190462,1.0193421,1.019638,
	1.0199339,1.02023,1.0205262,1.0208225,1.0211189,1.0214152,1.0217118,1.0220084,
	1.0223051,1.0226018,1.0228987,1.0231956,1.0234927,1.0237899,1.0240871,1.0243844,
	1.0246817,1.0249792,1.0252768,1.0255744,1.0258721,1.0261699,1.0264678,1.0267658,
	1.027064,1.0273621,1.0276604,1.0279586,1.0282571,1.0285556,1.0288541,
};
