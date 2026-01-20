import re

string = """
250, 0xFA,Steuerung Fensterheber FAT [10],0,0x0,KGM
251, 0xFB,Steuerung Fensterheber BFT [5],0,0x0,KGM
252, 0xFC,Steuerung Fensterheber FATH [5],114,0x72,KBM
253, 0xFD,Steuerung Fensterheber BFTH [5],114,0x72,KBM
"""

result = re.findall(r", (.*?)\,\s*(.*?)\,", string)
print([item[1] for item in result])
# Output: ['Steuerung Fensterheber FAT', 'Steuerung Fensterheber BFT', 
#          'Steuerung Fensterheber FATH', 'Steuerung Fensterheber BFTH']
