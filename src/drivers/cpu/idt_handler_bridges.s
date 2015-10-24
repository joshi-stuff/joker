.intel_syntax noprefix


.section .data

.global idt_handlers
idt_handlers:
  .zero 256*4


.section .text

.global idt_handler_bridge_0
idt_handler_bridge_0:
	call dword ptr [idt_handlers + 0]
	iret

.global idt_handler_bridge_1
idt_handler_bridge_1:
	call dword ptr [idt_handlers + 1]
	iret

.global idt_handler_bridge_2
idt_handler_bridge_2:
	call dword ptr [idt_handlers + 2]
	iret

.global idt_handler_bridge_3
idt_handler_bridge_3:
	call dword ptr [idt_handlers + 3]
	iret

.global idt_handler_bridge_4
idt_handler_bridge_4:
	call dword ptr [idt_handlers + 4]
	iret

.global idt_handler_bridge_5
idt_handler_bridge_5:
	call dword ptr [idt_handlers + 5]
	iret

.global idt_handler_bridge_6
idt_handler_bridge_6:
	call dword ptr [idt_handlers + 6]
	iret

.global idt_handler_bridge_7
idt_handler_bridge_7:
	call dword ptr [idt_handlers + 7]
	iret

.global idt_handler_bridge_8
idt_handler_bridge_8:
	call dword ptr [idt_handlers + 8]
	iret

.global idt_handler_bridge_9
idt_handler_bridge_9:
	call dword ptr [idt_handlers + 9]
	iret

.global idt_handler_bridge_10
idt_handler_bridge_10:
	call dword ptr [idt_handlers + 10]
	iret

.global idt_handler_bridge_11
idt_handler_bridge_11:
	call dword ptr [idt_handlers + 11]
	iret

.global idt_handler_bridge_12
idt_handler_bridge_12:
	call dword ptr [idt_handlers + 12]
	iret

.global idt_handler_bridge_13
idt_handler_bridge_13:
	call dword ptr [idt_handlers + 13]
	iret

.global idt_handler_bridge_14
idt_handler_bridge_14:
	call dword ptr [idt_handlers + 14]
	iret

.global idt_handler_bridge_15
idt_handler_bridge_15:
	call dword ptr [idt_handlers + 15]
	iret

.global idt_handler_bridge_16
idt_handler_bridge_16:
	call dword ptr [idt_handlers + 16]
	iret

.global idt_handler_bridge_17
idt_handler_bridge_17:
	call dword ptr [idt_handlers + 17]
	iret

.global idt_handler_bridge_18
idt_handler_bridge_18:
	call dword ptr [idt_handlers + 18]
	iret

.global idt_handler_bridge_19
idt_handler_bridge_19:
	call dword ptr [idt_handlers + 19]
	iret

.global idt_handler_bridge_20
idt_handler_bridge_20:
	call dword ptr [idt_handlers + 20]
	iret

.global idt_handler_bridge_21
idt_handler_bridge_21:
	call dword ptr [idt_handlers + 21]
	iret

.global idt_handler_bridge_22
idt_handler_bridge_22:
	call dword ptr [idt_handlers + 22]
	iret

.global idt_handler_bridge_23
idt_handler_bridge_23:
	call dword ptr [idt_handlers + 23]
	iret

.global idt_handler_bridge_24
idt_handler_bridge_24:
	call dword ptr [idt_handlers + 24]
	iret

.global idt_handler_bridge_25
idt_handler_bridge_25:
	call dword ptr [idt_handlers + 25]
	iret

.global idt_handler_bridge_26
idt_handler_bridge_26:
	call dword ptr [idt_handlers + 26]
	iret

.global idt_handler_bridge_27
idt_handler_bridge_27:
	call dword ptr [idt_handlers + 27]
	iret

.global idt_handler_bridge_28
idt_handler_bridge_28:
	call dword ptr [idt_handlers + 28]
	iret

.global idt_handler_bridge_29
idt_handler_bridge_29:
	call dword ptr [idt_handlers + 29]
	iret

.global idt_handler_bridge_30
idt_handler_bridge_30:
	call dword ptr [idt_handlers + 30]
	iret

.global idt_handler_bridge_31
idt_handler_bridge_31:
	call dword ptr [idt_handlers + 31]
	iret

.global idt_handler_bridge_32
idt_handler_bridge_32:
	call dword ptr [idt_handlers + 32]
	iret

.global idt_handler_bridge_33
idt_handler_bridge_33:
	call dword ptr [idt_handlers + 33]
	iret

.global idt_handler_bridge_34
idt_handler_bridge_34:
	call dword ptr [idt_handlers + 34]
	iret

.global idt_handler_bridge_35
idt_handler_bridge_35:
	call dword ptr [idt_handlers + 35]
	iret

.global idt_handler_bridge_36
idt_handler_bridge_36:
	call dword ptr [idt_handlers + 36]
	iret

.global idt_handler_bridge_37
idt_handler_bridge_37:
	call dword ptr [idt_handlers + 37]
	iret

.global idt_handler_bridge_38
idt_handler_bridge_38:
	call dword ptr [idt_handlers + 38]
	iret

.global idt_handler_bridge_39
idt_handler_bridge_39:
	call dword ptr [idt_handlers + 39]
	iret

.global idt_handler_bridge_40
idt_handler_bridge_40:
	call dword ptr [idt_handlers + 40]
	iret

.global idt_handler_bridge_41
idt_handler_bridge_41:
	call dword ptr [idt_handlers + 41]
	iret

.global idt_handler_bridge_42
idt_handler_bridge_42:
	call dword ptr [idt_handlers + 42]
	iret

.global idt_handler_bridge_43
idt_handler_bridge_43:
	call dword ptr [idt_handlers + 43]
	iret

.global idt_handler_bridge_44
idt_handler_bridge_44:
	call dword ptr [idt_handlers + 44]
	iret

.global idt_handler_bridge_45
idt_handler_bridge_45:
	call dword ptr [idt_handlers + 45]
	iret

.global idt_handler_bridge_46
idt_handler_bridge_46:
	call dword ptr [idt_handlers + 46]
	iret

.global idt_handler_bridge_47
idt_handler_bridge_47:
	call dword ptr [idt_handlers + 47]
	iret

.global idt_handler_bridge_48
idt_handler_bridge_48:
	call dword ptr [idt_handlers + 48]
	iret

.global idt_handler_bridge_49
idt_handler_bridge_49:
	call dword ptr [idt_handlers + 49]
	iret

.global idt_handler_bridge_50
idt_handler_bridge_50:
	call dword ptr [idt_handlers + 50]
	iret

.global idt_handler_bridge_51
idt_handler_bridge_51:
	call dword ptr [idt_handlers + 51]
	iret

.global idt_handler_bridge_52
idt_handler_bridge_52:
	call dword ptr [idt_handlers + 52]
	iret

.global idt_handler_bridge_53
idt_handler_bridge_53:
	call dword ptr [idt_handlers + 53]
	iret

.global idt_handler_bridge_54
idt_handler_bridge_54:
	call dword ptr [idt_handlers + 54]
	iret

.global idt_handler_bridge_55
idt_handler_bridge_55:
	call dword ptr [idt_handlers + 55]
	iret

.global idt_handler_bridge_56
idt_handler_bridge_56:
	call dword ptr [idt_handlers + 56]
	iret

.global idt_handler_bridge_57
idt_handler_bridge_57:
	call dword ptr [idt_handlers + 57]
	iret

.global idt_handler_bridge_58
idt_handler_bridge_58:
	call dword ptr [idt_handlers + 58]
	iret

.global idt_handler_bridge_59
idt_handler_bridge_59:
	call dword ptr [idt_handlers + 59]
	iret

.global idt_handler_bridge_60
idt_handler_bridge_60:
	call dword ptr [idt_handlers + 60]
	iret

.global idt_handler_bridge_61
idt_handler_bridge_61:
	call dword ptr [idt_handlers + 61]
	iret

.global idt_handler_bridge_62
idt_handler_bridge_62:
	call dword ptr [idt_handlers + 62]
	iret

.global idt_handler_bridge_63
idt_handler_bridge_63:
	call dword ptr [idt_handlers + 63]
	iret

.global idt_handler_bridge_64
idt_handler_bridge_64:
	call dword ptr [idt_handlers + 64]
	iret

.global idt_handler_bridge_65
idt_handler_bridge_65:
	call dword ptr [idt_handlers + 65]
	iret

.global idt_handler_bridge_66
idt_handler_bridge_66:
	call dword ptr [idt_handlers + 66]
	iret

.global idt_handler_bridge_67
idt_handler_bridge_67:
	call dword ptr [idt_handlers + 67]
	iret

.global idt_handler_bridge_68
idt_handler_bridge_68:
	call dword ptr [idt_handlers + 68]
	iret

.global idt_handler_bridge_69
idt_handler_bridge_69:
	call dword ptr [idt_handlers + 69]
	iret

.global idt_handler_bridge_70
idt_handler_bridge_70:
	call dword ptr [idt_handlers + 70]
	iret

.global idt_handler_bridge_71
idt_handler_bridge_71:
	call dword ptr [idt_handlers + 71]
	iret

.global idt_handler_bridge_72
idt_handler_bridge_72:
	call dword ptr [idt_handlers + 72]
	iret

.global idt_handler_bridge_73
idt_handler_bridge_73:
	call dword ptr [idt_handlers + 73]
	iret

.global idt_handler_bridge_74
idt_handler_bridge_74:
	call dword ptr [idt_handlers + 74]
	iret

.global idt_handler_bridge_75
idt_handler_bridge_75:
	call dword ptr [idt_handlers + 75]
	iret

.global idt_handler_bridge_76
idt_handler_bridge_76:
	call dword ptr [idt_handlers + 76]
	iret

.global idt_handler_bridge_77
idt_handler_bridge_77:
	call dword ptr [idt_handlers + 77]
	iret

.global idt_handler_bridge_78
idt_handler_bridge_78:
	call dword ptr [idt_handlers + 78]
	iret

.global idt_handler_bridge_79
idt_handler_bridge_79:
	call dword ptr [idt_handlers + 79]
	iret

.global idt_handler_bridge_80
idt_handler_bridge_80:
	call dword ptr [idt_handlers + 80]
	iret

.global idt_handler_bridge_81
idt_handler_bridge_81:
	call dword ptr [idt_handlers + 81]
	iret

.global idt_handler_bridge_82
idt_handler_bridge_82:
	call dword ptr [idt_handlers + 82]
	iret

.global idt_handler_bridge_83
idt_handler_bridge_83:
	call dword ptr [idt_handlers + 83]
	iret

.global idt_handler_bridge_84
idt_handler_bridge_84:
	call dword ptr [idt_handlers + 84]
	iret

.global idt_handler_bridge_85
idt_handler_bridge_85:
	call dword ptr [idt_handlers + 85]
	iret

.global idt_handler_bridge_86
idt_handler_bridge_86:
	call dword ptr [idt_handlers + 86]
	iret

.global idt_handler_bridge_87
idt_handler_bridge_87:
	call dword ptr [idt_handlers + 87]
	iret

.global idt_handler_bridge_88
idt_handler_bridge_88:
	call dword ptr [idt_handlers + 88]
	iret

.global idt_handler_bridge_89
idt_handler_bridge_89:
	call dword ptr [idt_handlers + 89]
	iret

.global idt_handler_bridge_90
idt_handler_bridge_90:
	call dword ptr [idt_handlers + 90]
	iret

.global idt_handler_bridge_91
idt_handler_bridge_91:
	call dword ptr [idt_handlers + 91]
	iret

.global idt_handler_bridge_92
idt_handler_bridge_92:
	call dword ptr [idt_handlers + 92]
	iret

.global idt_handler_bridge_93
idt_handler_bridge_93:
	call dword ptr [idt_handlers + 93]
	iret

.global idt_handler_bridge_94
idt_handler_bridge_94:
	call dword ptr [idt_handlers + 94]
	iret

.global idt_handler_bridge_95
idt_handler_bridge_95:
	call dword ptr [idt_handlers + 95]
	iret

.global idt_handler_bridge_96
idt_handler_bridge_96:
	call dword ptr [idt_handlers + 96]
	iret

.global idt_handler_bridge_97
idt_handler_bridge_97:
	call dword ptr [idt_handlers + 97]
	iret

.global idt_handler_bridge_98
idt_handler_bridge_98:
	call dword ptr [idt_handlers + 98]
	iret

.global idt_handler_bridge_99
idt_handler_bridge_99:
	call dword ptr [idt_handlers + 99]
	iret

.global idt_handler_bridge_100
idt_handler_bridge_100:
	call dword ptr [idt_handlers + 100]
	iret

.global idt_handler_bridge_101
idt_handler_bridge_101:
	call dword ptr [idt_handlers + 101]
	iret

.global idt_handler_bridge_102
idt_handler_bridge_102:
	call dword ptr [idt_handlers + 102]
	iret

.global idt_handler_bridge_103
idt_handler_bridge_103:
	call dword ptr [idt_handlers + 103]
	iret

.global idt_handler_bridge_104
idt_handler_bridge_104:
	call dword ptr [idt_handlers + 104]
	iret

.global idt_handler_bridge_105
idt_handler_bridge_105:
	call dword ptr [idt_handlers + 105]
	iret

.global idt_handler_bridge_106
idt_handler_bridge_106:
	call dword ptr [idt_handlers + 106]
	iret

.global idt_handler_bridge_107
idt_handler_bridge_107:
	call dword ptr [idt_handlers + 107]
	iret

.global idt_handler_bridge_108
idt_handler_bridge_108:
	call dword ptr [idt_handlers + 108]
	iret

.global idt_handler_bridge_109
idt_handler_bridge_109:
	call dword ptr [idt_handlers + 109]
	iret

.global idt_handler_bridge_110
idt_handler_bridge_110:
	call dword ptr [idt_handlers + 110]
	iret

.global idt_handler_bridge_111
idt_handler_bridge_111:
	call dword ptr [idt_handlers + 111]
	iret

.global idt_handler_bridge_112
idt_handler_bridge_112:
	call dword ptr [idt_handlers + 112]
	iret

.global idt_handler_bridge_113
idt_handler_bridge_113:
	call dword ptr [idt_handlers + 113]
	iret

.global idt_handler_bridge_114
idt_handler_bridge_114:
	call dword ptr [idt_handlers + 114]
	iret

.global idt_handler_bridge_115
idt_handler_bridge_115:
	call dword ptr [idt_handlers + 115]
	iret

.global idt_handler_bridge_116
idt_handler_bridge_116:
	call dword ptr [idt_handlers + 116]
	iret

.global idt_handler_bridge_117
idt_handler_bridge_117:
	call dword ptr [idt_handlers + 117]
	iret

.global idt_handler_bridge_118
idt_handler_bridge_118:
	call dword ptr [idt_handlers + 118]
	iret

.global idt_handler_bridge_119
idt_handler_bridge_119:
	call dword ptr [idt_handlers + 119]
	iret

.global idt_handler_bridge_120
idt_handler_bridge_120:
	call dword ptr [idt_handlers + 120]
	iret

.global idt_handler_bridge_121
idt_handler_bridge_121:
	call dword ptr [idt_handlers + 121]
	iret

.global idt_handler_bridge_122
idt_handler_bridge_122:
	call dword ptr [idt_handlers + 122]
	iret

.global idt_handler_bridge_123
idt_handler_bridge_123:
	call dword ptr [idt_handlers + 123]
	iret

.global idt_handler_bridge_124
idt_handler_bridge_124:
	call dword ptr [idt_handlers + 124]
	iret

.global idt_handler_bridge_125
idt_handler_bridge_125:
	call dword ptr [idt_handlers + 125]
	iret

.global idt_handler_bridge_126
idt_handler_bridge_126:
	call dword ptr [idt_handlers + 126]
	iret

.global idt_handler_bridge_127
idt_handler_bridge_127:
	call dword ptr [idt_handlers + 127]
	iret

.global idt_handler_bridge_128
idt_handler_bridge_128:
	call dword ptr [idt_handlers + 128]
	iret

.global idt_handler_bridge_129
idt_handler_bridge_129:
	call dword ptr [idt_handlers + 129]
	iret

.global idt_handler_bridge_130
idt_handler_bridge_130:
	call dword ptr [idt_handlers + 130]
	iret

.global idt_handler_bridge_131
idt_handler_bridge_131:
	call dword ptr [idt_handlers + 131]
	iret

.global idt_handler_bridge_132
idt_handler_bridge_132:
	call dword ptr [idt_handlers + 132]
	iret

.global idt_handler_bridge_133
idt_handler_bridge_133:
	call dword ptr [idt_handlers + 133]
	iret

.global idt_handler_bridge_134
idt_handler_bridge_134:
	call dword ptr [idt_handlers + 134]
	iret

.global idt_handler_bridge_135
idt_handler_bridge_135:
	call dword ptr [idt_handlers + 135]
	iret

.global idt_handler_bridge_136
idt_handler_bridge_136:
	call dword ptr [idt_handlers + 136]
	iret

.global idt_handler_bridge_137
idt_handler_bridge_137:
	call dword ptr [idt_handlers + 137]
	iret

.global idt_handler_bridge_138
idt_handler_bridge_138:
	call dword ptr [idt_handlers + 138]
	iret

.global idt_handler_bridge_139
idt_handler_bridge_139:
	call dword ptr [idt_handlers + 139]
	iret

.global idt_handler_bridge_140
idt_handler_bridge_140:
	call dword ptr [idt_handlers + 140]
	iret

.global idt_handler_bridge_141
idt_handler_bridge_141:
	call dword ptr [idt_handlers + 141]
	iret

.global idt_handler_bridge_142
idt_handler_bridge_142:
	call dword ptr [idt_handlers + 142]
	iret

.global idt_handler_bridge_143
idt_handler_bridge_143:
	call dword ptr [idt_handlers + 143]
	iret

.global idt_handler_bridge_144
idt_handler_bridge_144:
	call dword ptr [idt_handlers + 144]
	iret

.global idt_handler_bridge_145
idt_handler_bridge_145:
	call dword ptr [idt_handlers + 145]
	iret

.global idt_handler_bridge_146
idt_handler_bridge_146:
	call dword ptr [idt_handlers + 146]
	iret

.global idt_handler_bridge_147
idt_handler_bridge_147:
	call dword ptr [idt_handlers + 147]
	iret

.global idt_handler_bridge_148
idt_handler_bridge_148:
	call dword ptr [idt_handlers + 148]
	iret

.global idt_handler_bridge_149
idt_handler_bridge_149:
	call dword ptr [idt_handlers + 149]
	iret

.global idt_handler_bridge_150
idt_handler_bridge_150:
	call dword ptr [idt_handlers + 150]
	iret

.global idt_handler_bridge_151
idt_handler_bridge_151:
	call dword ptr [idt_handlers + 151]
	iret

.global idt_handler_bridge_152
idt_handler_bridge_152:
	call dword ptr [idt_handlers + 152]
	iret

.global idt_handler_bridge_153
idt_handler_bridge_153:
	call dword ptr [idt_handlers + 153]
	iret

.global idt_handler_bridge_154
idt_handler_bridge_154:
	call dword ptr [idt_handlers + 154]
	iret

.global idt_handler_bridge_155
idt_handler_bridge_155:
	call dword ptr [idt_handlers + 155]
	iret

.global idt_handler_bridge_156
idt_handler_bridge_156:
	call dword ptr [idt_handlers + 156]
	iret

.global idt_handler_bridge_157
idt_handler_bridge_157:
	call dword ptr [idt_handlers + 157]
	iret

.global idt_handler_bridge_158
idt_handler_bridge_158:
	call dword ptr [idt_handlers + 158]
	iret

.global idt_handler_bridge_159
idt_handler_bridge_159:
	call dword ptr [idt_handlers + 159]
	iret

.global idt_handler_bridge_160
idt_handler_bridge_160:
	call dword ptr [idt_handlers + 160]
	iret

.global idt_handler_bridge_161
idt_handler_bridge_161:
	call dword ptr [idt_handlers + 161]
	iret

.global idt_handler_bridge_162
idt_handler_bridge_162:
	call dword ptr [idt_handlers + 162]
	iret

.global idt_handler_bridge_163
idt_handler_bridge_163:
	call dword ptr [idt_handlers + 163]
	iret

.global idt_handler_bridge_164
idt_handler_bridge_164:
	call dword ptr [idt_handlers + 164]
	iret

.global idt_handler_bridge_165
idt_handler_bridge_165:
	call dword ptr [idt_handlers + 165]
	iret

.global idt_handler_bridge_166
idt_handler_bridge_166:
	call dword ptr [idt_handlers + 166]
	iret

.global idt_handler_bridge_167
idt_handler_bridge_167:
	call dword ptr [idt_handlers + 167]
	iret

.global idt_handler_bridge_168
idt_handler_bridge_168:
	call dword ptr [idt_handlers + 168]
	iret

.global idt_handler_bridge_169
idt_handler_bridge_169:
	call dword ptr [idt_handlers + 169]
	iret

.global idt_handler_bridge_170
idt_handler_bridge_170:
	call dword ptr [idt_handlers + 170]
	iret

.global idt_handler_bridge_171
idt_handler_bridge_171:
	call dword ptr [idt_handlers + 171]
	iret

.global idt_handler_bridge_172
idt_handler_bridge_172:
	call dword ptr [idt_handlers + 172]
	iret

.global idt_handler_bridge_173
idt_handler_bridge_173:
	call dword ptr [idt_handlers + 173]
	iret

.global idt_handler_bridge_174
idt_handler_bridge_174:
	call dword ptr [idt_handlers + 174]
	iret

.global idt_handler_bridge_175
idt_handler_bridge_175:
	call dword ptr [idt_handlers + 175]
	iret

.global idt_handler_bridge_176
idt_handler_bridge_176:
	call dword ptr [idt_handlers + 176]
	iret

.global idt_handler_bridge_177
idt_handler_bridge_177:
	call dword ptr [idt_handlers + 177]
	iret

.global idt_handler_bridge_178
idt_handler_bridge_178:
	call dword ptr [idt_handlers + 178]
	iret

.global idt_handler_bridge_179
idt_handler_bridge_179:
	call dword ptr [idt_handlers + 179]
	iret

.global idt_handler_bridge_180
idt_handler_bridge_180:
	call dword ptr [idt_handlers + 180]
	iret

.global idt_handler_bridge_181
idt_handler_bridge_181:
	call dword ptr [idt_handlers + 181]
	iret

.global idt_handler_bridge_182
idt_handler_bridge_182:
	call dword ptr [idt_handlers + 182]
	iret

.global idt_handler_bridge_183
idt_handler_bridge_183:
	call dword ptr [idt_handlers + 183]
	iret

.global idt_handler_bridge_184
idt_handler_bridge_184:
	call dword ptr [idt_handlers + 184]
	iret

.global idt_handler_bridge_185
idt_handler_bridge_185:
	call dword ptr [idt_handlers + 185]
	iret

.global idt_handler_bridge_186
idt_handler_bridge_186:
	call dword ptr [idt_handlers + 186]
	iret

.global idt_handler_bridge_187
idt_handler_bridge_187:
	call dword ptr [idt_handlers + 187]
	iret

.global idt_handler_bridge_188
idt_handler_bridge_188:
	call dword ptr [idt_handlers + 188]
	iret

.global idt_handler_bridge_189
idt_handler_bridge_189:
	call dword ptr [idt_handlers + 189]
	iret

.global idt_handler_bridge_190
idt_handler_bridge_190:
	call dword ptr [idt_handlers + 190]
	iret

.global idt_handler_bridge_191
idt_handler_bridge_191:
	call dword ptr [idt_handlers + 191]
	iret

.global idt_handler_bridge_192
idt_handler_bridge_192:
	call dword ptr [idt_handlers + 192]
	iret

.global idt_handler_bridge_193
idt_handler_bridge_193:
	call dword ptr [idt_handlers + 193]
	iret

.global idt_handler_bridge_194
idt_handler_bridge_194:
	call dword ptr [idt_handlers + 194]
	iret

.global idt_handler_bridge_195
idt_handler_bridge_195:
	call dword ptr [idt_handlers + 195]
	iret

.global idt_handler_bridge_196
idt_handler_bridge_196:
	call dword ptr [idt_handlers + 196]
	iret

.global idt_handler_bridge_197
idt_handler_bridge_197:
	call dword ptr [idt_handlers + 197]
	iret

.global idt_handler_bridge_198
idt_handler_bridge_198:
	call dword ptr [idt_handlers + 198]
	iret

.global idt_handler_bridge_199
idt_handler_bridge_199:
	call dword ptr [idt_handlers + 199]
	iret

.global idt_handler_bridge_200
idt_handler_bridge_200:
	call dword ptr [idt_handlers + 200]
	iret

.global idt_handler_bridge_201
idt_handler_bridge_201:
	call dword ptr [idt_handlers + 201]
	iret

.global idt_handler_bridge_202
idt_handler_bridge_202:
	call dword ptr [idt_handlers + 202]
	iret

.global idt_handler_bridge_203
idt_handler_bridge_203:
	call dword ptr [idt_handlers + 203]
	iret

.global idt_handler_bridge_204
idt_handler_bridge_204:
	call dword ptr [idt_handlers + 204]
	iret

.global idt_handler_bridge_205
idt_handler_bridge_205:
	call dword ptr [idt_handlers + 205]
	iret

.global idt_handler_bridge_206
idt_handler_bridge_206:
	call dword ptr [idt_handlers + 206]
	iret

.global idt_handler_bridge_207
idt_handler_bridge_207:
	call dword ptr [idt_handlers + 207]
	iret

.global idt_handler_bridge_208
idt_handler_bridge_208:
	call dword ptr [idt_handlers + 208]
	iret

.global idt_handler_bridge_209
idt_handler_bridge_209:
	call dword ptr [idt_handlers + 209]
	iret

.global idt_handler_bridge_210
idt_handler_bridge_210:
	call dword ptr [idt_handlers + 210]
	iret

.global idt_handler_bridge_211
idt_handler_bridge_211:
	call dword ptr [idt_handlers + 211]
	iret

.global idt_handler_bridge_212
idt_handler_bridge_212:
	call dword ptr [idt_handlers + 212]
	iret

.global idt_handler_bridge_213
idt_handler_bridge_213:
	call dword ptr [idt_handlers + 213]
	iret

.global idt_handler_bridge_214
idt_handler_bridge_214:
	call dword ptr [idt_handlers + 214]
	iret

.global idt_handler_bridge_215
idt_handler_bridge_215:
	call dword ptr [idt_handlers + 215]
	iret

.global idt_handler_bridge_216
idt_handler_bridge_216:
	call dword ptr [idt_handlers + 216]
	iret

.global idt_handler_bridge_217
idt_handler_bridge_217:
	call dword ptr [idt_handlers + 217]
	iret

.global idt_handler_bridge_218
idt_handler_bridge_218:
	call dword ptr [idt_handlers + 218]
	iret

.global idt_handler_bridge_219
idt_handler_bridge_219:
	call dword ptr [idt_handlers + 219]
	iret

.global idt_handler_bridge_220
idt_handler_bridge_220:
	call dword ptr [idt_handlers + 220]
	iret

.global idt_handler_bridge_221
idt_handler_bridge_221:
	call dword ptr [idt_handlers + 221]
	iret

.global idt_handler_bridge_222
idt_handler_bridge_222:
	call dword ptr [idt_handlers + 222]
	iret

.global idt_handler_bridge_223
idt_handler_bridge_223:
	call dword ptr [idt_handlers + 223]
	iret

.global idt_handler_bridge_224
idt_handler_bridge_224:
	call dword ptr [idt_handlers + 224]
	iret

.global idt_handler_bridge_225
idt_handler_bridge_225:
	call dword ptr [idt_handlers + 225]
	iret

.global idt_handler_bridge_226
idt_handler_bridge_226:
	call dword ptr [idt_handlers + 226]
	iret

.global idt_handler_bridge_227
idt_handler_bridge_227:
	call dword ptr [idt_handlers + 227]
	iret

.global idt_handler_bridge_228
idt_handler_bridge_228:
	call dword ptr [idt_handlers + 228]
	iret

.global idt_handler_bridge_229
idt_handler_bridge_229:
	call dword ptr [idt_handlers + 229]
	iret

.global idt_handler_bridge_230
idt_handler_bridge_230:
	call dword ptr [idt_handlers + 230]
	iret

.global idt_handler_bridge_231
idt_handler_bridge_231:
	call dword ptr [idt_handlers + 231]
	iret

.global idt_handler_bridge_232
idt_handler_bridge_232:
	call dword ptr [idt_handlers + 232]
	iret

.global idt_handler_bridge_233
idt_handler_bridge_233:
	call dword ptr [idt_handlers + 233]
	iret

.global idt_handler_bridge_234
idt_handler_bridge_234:
	call dword ptr [idt_handlers + 234]
	iret

.global idt_handler_bridge_235
idt_handler_bridge_235:
	call dword ptr [idt_handlers + 235]
	iret

.global idt_handler_bridge_236
idt_handler_bridge_236:
	call dword ptr [idt_handlers + 236]
	iret

.global idt_handler_bridge_237
idt_handler_bridge_237:
	call dword ptr [idt_handlers + 237]
	iret

.global idt_handler_bridge_238
idt_handler_bridge_238:
	call dword ptr [idt_handlers + 238]
	iret

.global idt_handler_bridge_239
idt_handler_bridge_239:
	call dword ptr [idt_handlers + 239]
	iret

.global idt_handler_bridge_240
idt_handler_bridge_240:
	call dword ptr [idt_handlers + 240]
	iret

.global idt_handler_bridge_241
idt_handler_bridge_241:
	call dword ptr [idt_handlers + 241]
	iret

.global idt_handler_bridge_242
idt_handler_bridge_242:
	call dword ptr [idt_handlers + 242]
	iret

.global idt_handler_bridge_243
idt_handler_bridge_243:
	call dword ptr [idt_handlers + 243]
	iret

.global idt_handler_bridge_244
idt_handler_bridge_244:
	call dword ptr [idt_handlers + 244]
	iret

.global idt_handler_bridge_245
idt_handler_bridge_245:
	call dword ptr [idt_handlers + 245]
	iret

.global idt_handler_bridge_246
idt_handler_bridge_246:
	call dword ptr [idt_handlers + 246]
	iret

.global idt_handler_bridge_247
idt_handler_bridge_247:
	call dword ptr [idt_handlers + 247]
	iret

.global idt_handler_bridge_248
idt_handler_bridge_248:
	call dword ptr [idt_handlers + 248]
	iret

.global idt_handler_bridge_249
idt_handler_bridge_249:
	call dword ptr [idt_handlers + 249]
	iret

.global idt_handler_bridge_250
idt_handler_bridge_250:
	call dword ptr [idt_handlers + 250]
	iret

.global idt_handler_bridge_251
idt_handler_bridge_251:
	call dword ptr [idt_handlers + 251]
	iret

.global idt_handler_bridge_252
idt_handler_bridge_252:
	call dword ptr [idt_handlers + 252]
	iret

.global idt_handler_bridge_253
idt_handler_bridge_253:
	call dword ptr [idt_handlers + 253]
	iret

.global idt_handler_bridge_254
idt_handler_bridge_254:
	call dword ptr [idt_handlers + 254]
	iret

.global idt_handler_bridge_255
idt_handler_bridge_255:
	call dword ptr [idt_handlers + 255]
	iret
