from math import *

def initializeConstants(states, variables):
    states[0] = 0.092361701692
    states[1] = 0.015905380261
    states[2] = 0.01445216109
    states[3] = 0.04804900895
    states[4] = 0.48779845203
    states[5] = 0.42074047435
    states[6] = 0.038968420558
    states[7] = 0.29760539675
    states[8] = 0.064402950262
    states[9] = 0.13034201158
    states[10] = 0.46960956028
    states[11] = 0.87993375273
    states[12] = 0.082293827208
    states[13] = 0.03889291759
    states[14] = -39.013558536
    variables[0] = 46.4
    variables[1] = 0.0057938
    variables[2] = 0.0082
    variables[3] = 0.0058
    variables[4] = 0.06588648
    variables[5] = 0.0659
    variables[6] = 0.0659
    variables[7] = 45.0
    variables[8] = 0.00427806
    variables[9] = 0.0021
    variables[10] = 0.0043
    variables[11] = 0.0138823
    variables[12] = 0.00694
    variables[13] = 0.0139
    variables[14] = 1.3236e-5
    variables[15] = 1.323e-5
    variables[16] = 1.32e-5
    variables[17] = 4.2952e-5
    variables[18] = 4.29e-5
    variables[19] = 4.3e-5
    variables[20] = 6.645504e-5
    variables[21] = 0.000266
    variables[22] = 6.65e-5
    variables[23] = 0.01138376
    variables[24] = 0.0114
    variables[25] = 0.0114
    variables[26] = 0.004905
    variables[27] = 0.004905
    variables[28] = 0.00491
    variables[29] = 0.036495
    variables[30] = 0.0365
    variables[31] = 0.03649
    variables[32] = 0.0005465
    variables[33] = 0.000437
    variables[34] = 0.000548
    variables[35] = 0.006875
    variables[36] = 0.0055
    variables[37] = 0.0069
    variables[38] = 0.0005465
    variables[39] = 0.000437
    variables[40] = 0.000548
    variables[41] = 0.006875
    variables[42] = 0.0055
    variables[43] = 0.0069
    variables[44] = 0.0001
    variables[45] = 2.0
    variables[46] = 140.0
    variables[47] = 5.4
    variables[48] = 8.0
    variables[49] = 140.0
    variables[50] = 2.0e-5
    variables[51] = 6.5e-5
    variables[52] = 96845.0
    variables[53] = 1.0309347
    variables[54] = 8314.0
    variables[55] = 310.0
    variables[56] = 1.0
    variables[57] = 0.0
    variables[58] = 0.0
    variables[59] = 0.0042
    variables[60] = 0.0
    variables[61] = 0.0
    variables[62] = 0.03339
    variables[63] = 0.0
    variables[64] = 2.523636e-5
    variables[65] = 2.52e-5
    variables[66] = 2.52e-5
    variables[67] = 8.1892e-5
    variables[68] = 8.19e-5
    variables[69] = 8.19e-5
    variables[70] = 0.00079704
    variables[71] = 0.000738
    variables[72] = 0.000797
    variables[73] = 0.016
    variables[74] = 0.0208
    variables[75] = 0.016
    variables[76] = 0.0003445
    variables[77] = 0.000345
    variables[78] = 0.000518
    variables[79] = 0.0104
    variables[80] = 0.0104
    variables[81] = 0.0104
    variables[82] = 5.81818e-5
    variables[83] = 5.8e-5
    variables[84] = 5.8e-5
    variables[85] = 0.0001888
    variables[86] = 0.000189
    variables[87] = 0.000189
    variables[88] = 0.0001
    variables[89] = 0.5
    variables[90] = 2.7229e-6
    variables[91] = 2.8e-6
    variables[92] = 2.7e-6
    variables[93] = 8.83584e-6
    variables[94] = 8.8e-6
    variables[95] = 8.8e-6
    variables[96] = 0.0
    variables[97] = 0.0
    variables[98] = 0.0
    variables[99] = 1.204e-6
    variables[100] = 3.7e-7
    variables[101] = 1.2e-6
    variables[102] = 0.621
    variables[103] = 5.64
    variables[104] = 0.04782545
    variables[105] = 0.0478
    variables[106] = 0.0478
    variables[107] = 0.1551936
    variables[108] = 0.16
    variables[109] = 0.16

def computeComputedConstants(variables):
    variables[110] = (variables[56] == 0.0) if 1.07*(3.0*variables[57]-0.1)/(3.0*(1.0+0.7745*exp(-(3.0*variables[57]-2.05)/0.295))) else (variables[56] == 1.0) if variables[53]*variables[57]/(1.0+0.7745*exp(-(3.0*variables[57]-2.05)/0.295)) else 1.07*29.0*variables[57]/(30.0*(1.0+0.7745*exp(-(29.0*variables[57]-24.5)/1.95)))
    variables[111] = variables[50]+variables[110]*(variables[51]-variables[50])
    variables[112] = (variables[56] == 0.0) if variables[84]+variables[110]*(variables[87]-variables[84]) else (variables[56] == 1.0) if variables[82]+variables[110]*(variables[85]-variables[82]) else variables[83]+variables[110]*(variables[86]-variables[83])
    variables[113] = (variables[56] == 0.0) if variables[66]+variables[110]*(variables[69]-variables[66]) else (variables[56] == 1.0) if variables[64]+variables[110]*(variables[67]-variables[64]) else variables[65]+variables[110]*(variables[68]-variables[65])
    variables[114] = (variables[56] == 0.0) if variables[16]+variables[110]*(variables[19]-variables[16]) else (variables[56] == 1.0) if variables[14]+variables[110]*(variables[17]-variables[14]) else variables[15]+variables[110]*(variables[18]-variables[15])
    variables[115] = (variables[56] == 0.0) if variables[92]+variables[110]*(variables[95]-variables[92]) else (variables[56] == 1.0) if variables[90]+variables[110]*(variables[93]-variables[90]) else variables[91]+variables[110]*(variables[94]-variables[91])
    variables[117] = (variables[56] == 0.0) if variables[106]+variables[110]*(variables[109]-variables[106]) else (variables[56] == 1.0) if variables[104]+variables[110]*(variables[107]-variables[104]) else variables[105]+variables[110]*(variables[108]-variables[105])
    variables[119] = (variables[56] == 0.0) if variables[60]+variables[110]*(variables[63]-variables[60]) else (variables[56] == 1.0) if variables[58]+variables[110]*(variables[61]-variables[58]) else variables[59]+variables[110]*(variables[62]-variables[59])
    variables[120] = variables[119]*variables[44]/(variables[44]+0.0004)
    variables[121] = variables[54]*variables[55]/variables[52]*log(variables[49]/variables[48])
    variables[122] = variables[54]*variables[55]/variables[52]*log(variables[47]/variables[46])
    variables[123] = variables[54]*variables[55]/(2.0*variables[52])*log(variables[45]/variables[44])
    variables[124] = (variables[56] == 0.0) if variables[54]*variables[55]/variables[52]*log((variables[47]+0.12*variables[49])/(variables[46]+0.12*variables[48])) else variables[54]*variables[55]/variables[52]*log((variables[47]+0.03*variables[49])/(variables[46]+0.03*variables[48]))
    variables[125] = (variables[56] == 0.0) if variables[98]+variables[110]*(variables[101]-variables[98]) else (variables[56] == 1.0) if variables[96]+variables[110]*(variables[99]-variables[96]) else variables[97]+variables[110]*(variables[100]-variables[97])
    variables[134] = (variables[56] == 0.0) if variables[3]+variables[110]*(variables[6]-variables[3]) else (variables[56] == 1.0) if variables[1]+variables[110]*(variables[4]-variables[1]) else variables[2]+variables[110]*(variables[5]-variables[2])
    variables[144] = (variables[56] == 0.0) if variables[10]+variables[110]*(variables[13]-variables[10]) else (variables[56] == 1.0) if variables[8]+variables[110]*(variables[11]-variables[8]) else variables[9]+variables[110]*(variables[12]-variables[9])
    variables[154] = (variables[56] == 0.0) if variables[28]+variables[110]*(variables[31]-variables[28]) else (variables[56] == 1.0) if variables[26]+variables[110]*(variables[29]-variables[26]) else variables[27]+variables[110]*(variables[30]-variables[27])
    variables[155] = (variables[56] == 0.0) if variables[22]+variables[110]*(variables[25]-variables[22]) else (variables[56] == 1.0) if variables[20]+variables[110]*(variables[23]-variables[20]) else variables[21]+variables[110]*(variables[24]-variables[21])
    variables[162] = (variables[56] == 0.0) if variables[72]+variables[110]*(variables[75]-variables[72]) else (variables[56] == 1.0) if variables[70]+variables[110]*(variables[73]-variables[70]) else variables[71]+variables[110]*(variables[74]-variables[71])
    variables[168] = (variables[56] == 0.0) if 0.002 else (variables[56] == 1.0) if 0.002 else 0.006
    variables[170] = (variables[56] == 0.0) if variables[78]+variables[110]*(variables[81]-variables[78]) else (variables[56] == 1.0) if variables[76]+variables[110]*(variables[79]-variables[76]) else variables[77]+variables[110]*(variables[80]-variables[77])
    variables[174] = (variables[56] == 0.0) if variables[40]+variables[110]*(variables[43]-variables[40]) else (variables[56] == 1.0) if variables[38]+variables[110]*(variables[41]-variables[38]) else variables[39]+variables[110]*(variables[42]-variables[39])
    variables[176] = (variables[56] == 0.0) if variables[34]+variables[110]*(variables[37]-variables[34]) else (variables[56] == 1.0) if variables[32]+variables[110]*(variables[35]-variables[32]) else variables[33]+variables[110]*(variables[36]-variables[33])

def computeRates(voi, states, rates, variables):
    variables[126] = (variables[56] == 0.0) if pow(1.0/(1.0+exp(-states[14]/5.46)), 1.0/3.0) else pow(1.0/(1.0+exp(-(states[14]+30.32)/5.46)), 1.0/3.0)
    variables[127] = (variables[56] == 0.0) if 0.0006247/(0.832*exp(-0.335*(states[14]+56.7))+0.627*exp(0.082*(states[14]+65.01)))+4.0e-5 else 0.0006247/(0.8322166*exp(-0.33566*(states[14]+56.7062))+0.6274*exp(0.0823*(states[14]+65.0131)))+4.569e-5
    rates[0] = (variables[126]-states[0])/variables[127]
    variables[130] = 1.0/(1.0+exp((states[14]+66.1)/6.4))
    variables[132] = 3.717e-6*exp(-0.2815*(states[14]+17.11))/(1.0+0.003732*exp(-0.3426*(states[14]+37.76)))+0.0005977
    rates[1] = (variables[130]-states[1])/variables[132]
    variables[131] = variables[130]
    variables[133] = 3.186e-8*exp(-0.6219*(states[14]+18.8))/(1.0+7.189e-5*exp(-0.6683*(states[14]+34.07)))+0.003556
    rates[2] = (variables[131]-states[2])/variables[133]
    variables[139] = (variables[56] == 0.0) if 1.0/(1.0+exp(-(states[14]+23.1)/6.0)) else (variables[56] == 1.0) if 1.0/(1.0+exp(-(states[14]+22.3+0.8*variables[110])/6.0)) else 1.0/(1.0+exp(-(states[14]+22.2)/6.0))
    variables[136] = (variables[56] == 0.0) if -28.38*(states[14]+35.0)/(exp(-(states[14]+35.0)/2.5)-1.0)-84.9*states[14]/(exp(-0.208*states[14])-1.0) else (variables[56] == 1.0) if -28.39*(states[14]+35.0)/(exp(-(states[14]+35.0)/2.5)-1.0)-84.9*states[14]/(exp(-0.208*states[14])-1.0) else -28.4*(states[14]+35.0)/(exp(-(states[14]+35.0)/2.5)-1.0)-84.9*states[14]/(exp(-0.208*states[14])-1.0)
    variables[137] = (variables[56] == 1.0) if 11.43*(states[14]-5.0)/(exp(0.4*(states[14]-5.0))-1.0) else 11.42*(states[14]-5.0)/(exp(0.4*(states[14]-5.0))-1.0)
    variables[138] = 2.0/(variables[136]+variables[137])
    rates[3] = (variables[139]-states[3])/variables[138]
    variables[143] = 1.0/(1.0+exp((states[14]+45.0)/5.0))
    variables[140] = (variables[56] == 1.0) if 3.75*(states[14]+28.0)/(exp((states[14]+28.0)/4.0)-1.0) else 3.12*(states[14]+28.0)/(exp((states[14]+28.0)/4.0)-1.0)
    variables[141] = (variables[56] == 1.0) if 30.0/(1.0+exp(-(states[14]+28.0)/4.0)) else 25.0/(1.0+exp(-(states[14]+28.0)/4.0))
    variables[142] = (variables[56] == 1.0) if (1.2-0.2*variables[110])/(variables[140]+variables[141]) else 1.0/(variables[140]+variables[141])
    rates[4] = (variables[143]-states[4])/variables[142]
    variables[149] = 1.0/(1.0+exp(-(states[14]+37.0)/6.8))
    variables[146] = 1068.0*exp((states[14]+26.3)/30.0)
    variables[147] = 1068.0*exp(-(states[14]+26.3)/30.0)
    variables[148] = 1.0/(variables[146]+variables[147])
    rates[5] = (variables[149]-states[5])/variables[148]
    variables[153] = 1.0/(1.0+exp((states[14]+71.0)/9.0))
    variables[150] = (variables[56] == 1.0) if 15.3*exp(-(states[14]+71.0+0.7*variables[110])/83.3) else 15.3*exp(-(states[14]+71.7)/83.3)
    variables[151] = (variables[56] == 1.0) if 15.0*exp((states[14]+71.0)/15.38) else 15.0*exp((states[14]+71.7)/15.38)
    variables[152] = 1.0/(variables[150]+variables[151])
    rates[6] = (variables[153]-states[6])/variables[152]
    variables[158] = 1.0/(1.0+exp((states[14]+59.37)/13.1))
    variables[159] = (variables[56] == 0.0) if 0.0101+0.06517/(0.57*exp(-0.08*(states[14]+49.0)))+2.4e-5*exp(0.1*(states[14]+50.93)) else (variables[56] == 1.0) if 0.001/3.0*(30.31+195.5/(0.5686*exp(-0.08161*(states[14]+39.0+10.0*variables[110]))+0.7174*exp((0.2719-0.1719*variables[110])*1.0*(states[14]+40.93+10.0*variables[110])))) else 0.0101+0.06517/(0.5686*exp(-0.08161*(states[14]+39.0))+0.7174*exp(0.2719*(states[14]+40.93)))
    rates[7] = (variables[158]-states[7])/variables[159]
    variables[160] = 1.0/(1.0+exp(-(states[14]-10.93)/19.7))
    variables[161] = (variables[56] == 0.0) if 0.001*(2.98+15.59/(1.037*exp(0.09*(states[14]+30.61))+0.369*exp(-0.12*(states[14]+23.84)))) else (variables[56] == 1.0) if 0.0025*(1.191+7.838/(1.037*exp(0.09012*(states[14]+30.61))+0.369*exp(-0.119*(states[14]+23.84)))) else 0.001*(2.98+19.59/(1.037*exp(0.09012*(states[14]+30.61))+0.369*exp(-0.119*(states[14]+23.84))))
    rates[8] = (variables[160]-states[8])/variables[161]
    variables[164] = (variables[56] != 2.0) if 1.0/(1.0+exp(-(states[14]+14.2)/10.6)) else 1.0/(1.0+exp(-(states[14]+13.2)/10.6))
    variables[165] = (variables[56] != 2.0) if 1.0/(37.2*exp((states[14]-9.0)/15.9)+0.96*exp(-(states[14]-9.0)/22.5)) else 1.0/(37.2*exp((states[14]-10.0)/15.9)+0.96*exp(-(states[14]-10.0)/22.5))
    rates[9] = (variables[164]-states[9])/variables[165]
    variables[166] = variables[164]
    variables[167] = (variables[56] != 2.0) if 1.0/(4.2*exp((states[14]-9.0)/17.0)+0.15*exp(-(states[14]-9.0)/21.6)) else 1.0/(4.2*exp((states[14]-10.0)/17.0)+0.15*exp(-(states[14]-10.0)/21.6))
    rates[10] = (variables[166]-states[10])/variables[167]
    variables[169] = 1.0/(1.0+exp((states[14]+18.6)/10.1))
    rates[11] = (variables[169]-states[11])/variables[168]
    variables[172] = 14.0/(1.0+exp(-(states[14]-40.0)/9.0))
    variables[173] = 1.0*exp(-states[14]/45.0)
    rates[12] = variables[172]*(1.0-states[12])-variables[173]*states[12]
    variables[178] = (variables[56] == 0.0) if 1.0*exp(-(states[14]+78.91)/26.62) else 1.0*exp(-(states[14]+78.91)/26.63)
    variables[179] = 1.0*exp((states[14]+75.13)/21.25)
    rates[13] = variables[178]*(1.0-states[13])-variables[179]*states[13]
    variables[118] = variables[117]*pow(variables[48]/(variables[103]+variables[48]), 3.0)*pow(variables[47]/(variables[102]+variables[47]), 2.0)*1.6/(1.5+exp(-(states[14]+60.0)/40.0))
    variables[116] = (variables[56] == 0.0) if variables[115]*(pow(variables[48], 3.0)*variables[45]*exp(0.03743*states[14]*variables[89])-pow(variables[49], 3.0)*variables[44]*exp(0.0374*states[14]*(variables[89]-1.0)))/(1.0+variables[88]*(variables[44]*pow(variables[49], 3.0)+variables[45]*pow(variables[48], 3.0))) else variables[115]*(pow(variables[48], 3.0)*variables[45]*exp(0.03743*states[14]*variables[89])-pow(variables[49], 3.0)*variables[44]*exp(0.03743*states[14]*(variables[89]-1.0)))/(1.0+variables[88]*(variables[44]*pow(variables[49], 3.0)+variables[45]*pow(variables[48], 3.0)))
    variables[177] = (variables[56] != 2.0) if variables[176]*states[13]*(states[14]-variables[122]) else variables[176]*states[13]*(states[14]+102.0)
    variables[175] = (variables[56] != 2.0) if variables[174]*states[13]*(states[14]-variables[121]) else variables[174]*states[13]*(states[14]-77.6)
    variables[171] = variables[170]*pow(states[12], 2.0)*(states[14]-variables[124])
    variables[157] = variables[155]*states[8]*(states[14]-variables[122])
    variables[156] = variables[154]*states[7]*states[8]*(states[14]-variables[122])
    variables[145] = variables[144]*states[5]*states[6]*(states[14]-variables[7])
    variables[135] = variables[134]*(states[4]*states[3]+0.006/(1.0+exp(-(states[14]+14.1)/6.0)))*(states[14]-variables[0])
    variables[128] = (variables[56] == 0.0) if 0.0952*exp(-0.063*(states[14]+34.4))/(1.0+1.66*exp(-0.225*(states[14]+63.7)))+0.0869 else 0.09518*exp(-0.06306*(states[14]+34.4))/(1.0+1.662*exp(-0.2251*(states[14]+63.7)))+0.08693
    variables[129] = (1.0-variables[128])*states[1]+variables[128]*states[2]
    variables[183] = variables[125]*pow(states[0], 3.0)*variables[129]*variables[49]*pow(variables[52], 2.0)/(variables[54]*variables[55])*(exp((states[14]-variables[121])*variables[52]/(variables[54]*variables[55]))-1.0)/(exp(states[14]*variables[52]/(variables[54]*variables[55]))-1.0)*states[14]
    variables[181] = variables[113]*(states[14]-variables[122])
    variables[182] = variables[114]*(states[14]-variables[123])
    variables[180] = variables[112]*(states[14]-variables[121])
    variables[163] = 0.6*states[9]+0.4*states[10]
    variables[184] = variables[162]*variables[163]*states[11]*(states[14]-variables[122])
    rates[14] = -1.0/variables[111]*(variables[183]+variables[135]+variables[145]+variables[156]+variables[157]+variables[184]+variables[171]+variables[175]+variables[177]+variables[180]+variables[182]+variables[181]+variables[116]+variables[118]+variables[120])

def computeVariables(voi, states, rates, variables):
    variables[116] = (variables[56] == 0.0) if variables[115]*(pow(variables[48], 3.0)*variables[45]*exp(0.03743*states[14]*variables[89])-pow(variables[49], 3.0)*variables[44]*exp(0.0374*states[14]*(variables[89]-1.0)))/(1.0+variables[88]*(variables[44]*pow(variables[49], 3.0)+variables[45]*pow(variables[48], 3.0))) else variables[115]*(pow(variables[48], 3.0)*variables[45]*exp(0.03743*states[14]*variables[89])-pow(variables[49], 3.0)*variables[44]*exp(0.03743*states[14]*(variables[89]-1.0)))/(1.0+variables[88]*(variables[44]*pow(variables[49], 3.0)+variables[45]*pow(variables[48], 3.0)))
    variables[118] = variables[117]*pow(variables[48]/(variables[103]+variables[48]), 3.0)*pow(variables[47]/(variables[102]+variables[47]), 2.0)*1.6/(1.5+exp(-(states[14]+60.0)/40.0))
    variables[126] = (variables[56] == 0.0) if pow(1.0/(1.0+exp(-states[14]/5.46)), 1.0/3.0) else pow(1.0/(1.0+exp(-(states[14]+30.32)/5.46)), 1.0/3.0)
    variables[127] = (variables[56] == 0.0) if 0.0006247/(0.832*exp(-0.335*(states[14]+56.7))+0.627*exp(0.082*(states[14]+65.01)))+4.0e-5 else 0.0006247/(0.8322166*exp(-0.33566*(states[14]+56.7062))+0.6274*exp(0.0823*(states[14]+65.0131)))+4.569e-5
    variables[128] = (variables[56] == 0.0) if 0.0952*exp(-0.063*(states[14]+34.4))/(1.0+1.66*exp(-0.225*(states[14]+63.7)))+0.0869 else 0.09518*exp(-0.06306*(states[14]+34.4))/(1.0+1.662*exp(-0.2251*(states[14]+63.7)))+0.08693
    variables[129] = (1.0-variables[128])*states[1]+variables[128]*states[2]
    variables[130] = 1.0/(1.0+exp((states[14]+66.1)/6.4))
    variables[131] = variables[130]
    variables[132] = 3.717e-6*exp(-0.2815*(states[14]+17.11))/(1.0+0.003732*exp(-0.3426*(states[14]+37.76)))+0.0005977
    variables[133] = 3.186e-8*exp(-0.6219*(states[14]+18.8))/(1.0+7.189e-5*exp(-0.6683*(states[14]+34.07)))+0.003556
    variables[135] = variables[134]*(states[4]*states[3]+0.006/(1.0+exp(-(states[14]+14.1)/6.0)))*(states[14]-variables[0])
    variables[136] = (variables[56] == 0.0) if -28.38*(states[14]+35.0)/(exp(-(states[14]+35.0)/2.5)-1.0)-84.9*states[14]/(exp(-0.208*states[14])-1.0) else (variables[56] == 1.0) if -28.39*(states[14]+35.0)/(exp(-(states[14]+35.0)/2.5)-1.0)-84.9*states[14]/(exp(-0.208*states[14])-1.0) else -28.4*(states[14]+35.0)/(exp(-(states[14]+35.0)/2.5)-1.0)-84.9*states[14]/(exp(-0.208*states[14])-1.0)
    variables[137] = (variables[56] == 1.0) if 11.43*(states[14]-5.0)/(exp(0.4*(states[14]-5.0))-1.0) else 11.42*(states[14]-5.0)/(exp(0.4*(states[14]-5.0))-1.0)
    variables[138] = 2.0/(variables[136]+variables[137])
    variables[139] = (variables[56] == 0.0) if 1.0/(1.0+exp(-(states[14]+23.1)/6.0)) else (variables[56] == 1.0) if 1.0/(1.0+exp(-(states[14]+22.3+0.8*variables[110])/6.0)) else 1.0/(1.0+exp(-(states[14]+22.2)/6.0))
    variables[140] = (variables[56] == 1.0) if 3.75*(states[14]+28.0)/(exp((states[14]+28.0)/4.0)-1.0) else 3.12*(states[14]+28.0)/(exp((states[14]+28.0)/4.0)-1.0)
    variables[141] = (variables[56] == 1.0) if 30.0/(1.0+exp(-(states[14]+28.0)/4.0)) else 25.0/(1.0+exp(-(states[14]+28.0)/4.0))
    variables[142] = (variables[56] == 1.0) if (1.2-0.2*variables[110])/(variables[140]+variables[141]) else 1.0/(variables[140]+variables[141])
    variables[143] = 1.0/(1.0+exp((states[14]+45.0)/5.0))
    variables[145] = variables[144]*states[5]*states[6]*(states[14]-variables[7])
    variables[146] = 1068.0*exp((states[14]+26.3)/30.0)
    variables[147] = 1068.0*exp(-(states[14]+26.3)/30.0)
    variables[148] = 1.0/(variables[146]+variables[147])
    variables[149] = 1.0/(1.0+exp(-(states[14]+37.0)/6.8))
    variables[150] = (variables[56] == 1.0) if 15.3*exp(-(states[14]+71.0+0.7*variables[110])/83.3) else 15.3*exp(-(states[14]+71.7)/83.3)
    variables[151] = (variables[56] == 1.0) if 15.0*exp((states[14]+71.0)/15.38) else 15.0*exp((states[14]+71.7)/15.38)
    variables[152] = 1.0/(variables[150]+variables[151])
    variables[153] = 1.0/(1.0+exp((states[14]+71.0)/9.0))
    variables[156] = variables[154]*states[7]*states[8]*(states[14]-variables[122])
    variables[157] = variables[155]*states[8]*(states[14]-variables[122])
    variables[158] = 1.0/(1.0+exp((states[14]+59.37)/13.1))
    variables[159] = (variables[56] == 0.0) if 0.0101+0.06517/(0.57*exp(-0.08*(states[14]+49.0)))+2.4e-5*exp(0.1*(states[14]+50.93)) else (variables[56] == 1.0) if 0.001/3.0*(30.31+195.5/(0.5686*exp(-0.08161*(states[14]+39.0+10.0*variables[110]))+0.7174*exp((0.2719-0.1719*variables[110])*1.0*(states[14]+40.93+10.0*variables[110])))) else 0.0101+0.06517/(0.5686*exp(-0.08161*(states[14]+39.0))+0.7174*exp(0.2719*(states[14]+40.93)))
    variables[160] = 1.0/(1.0+exp(-(states[14]-10.93)/19.7))
    variables[161] = (variables[56] == 0.0) if 0.001*(2.98+15.59/(1.037*exp(0.09*(states[14]+30.61))+0.369*exp(-0.12*(states[14]+23.84)))) else (variables[56] == 1.0) if 0.0025*(1.191+7.838/(1.037*exp(0.09012*(states[14]+30.61))+0.369*exp(-0.119*(states[14]+23.84)))) else 0.001*(2.98+19.59/(1.037*exp(0.09012*(states[14]+30.61))+0.369*exp(-0.119*(states[14]+23.84))))
    variables[163] = 0.6*states[9]+0.4*states[10]
    variables[164] = (variables[56] != 2.0) if 1.0/(1.0+exp(-(states[14]+14.2)/10.6)) else 1.0/(1.0+exp(-(states[14]+13.2)/10.6))
    variables[165] = (variables[56] != 2.0) if 1.0/(37.2*exp((states[14]-9.0)/15.9)+0.96*exp(-(states[14]-9.0)/22.5)) else 1.0/(37.2*exp((states[14]-10.0)/15.9)+0.96*exp(-(states[14]-10.0)/22.5))
    variables[166] = variables[164]
    variables[167] = (variables[56] != 2.0) if 1.0/(4.2*exp((states[14]-9.0)/17.0)+0.15*exp(-(states[14]-9.0)/21.6)) else 1.0/(4.2*exp((states[14]-10.0)/17.0)+0.15*exp(-(states[14]-10.0)/21.6))
    variables[169] = 1.0/(1.0+exp((states[14]+18.6)/10.1))
    variables[171] = variables[170]*pow(states[12], 2.0)*(states[14]-variables[124])
    variables[172] = 14.0/(1.0+exp(-(states[14]-40.0)/9.0))
    variables[173] = 1.0*exp(-states[14]/45.0)
    variables[175] = (variables[56] != 2.0) if variables[174]*states[13]*(states[14]-variables[121]) else variables[174]*states[13]*(states[14]-77.6)
    variables[177] = (variables[56] != 2.0) if variables[176]*states[13]*(states[14]-variables[122]) else variables[176]*states[13]*(states[14]+102.0)
    variables[178] = (variables[56] == 0.0) if 1.0*exp(-(states[14]+78.91)/26.62) else 1.0*exp(-(states[14]+78.91)/26.63)
    variables[179] = 1.0*exp((states[14]+75.13)/21.25)
    variables[180] = variables[112]*(states[14]-variables[121])
    variables[181] = variables[113]*(states[14]-variables[122])
    variables[182] = variables[114]*(states[14]-variables[123])
    variables[183] = variables[125]*pow(states[0], 3.0)*variables[129]*variables[49]*pow(variables[52], 2.0)/(variables[54]*variables[55])*(exp((states[14]-variables[121])*variables[52]/(variables[54]*variables[55]))-1.0)/(exp(states[14]*variables[52]/(variables[54]*variables[55]))-1.0)*states[14]
    variables[184] = variables[162]*variables[163]*states[11]*(states[14]-variables[122])