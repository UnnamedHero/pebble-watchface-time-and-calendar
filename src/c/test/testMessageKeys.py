import json
import re

packageJson = '../../../package.json'
with open(packageJson) as data_file:
    data = json.load(data_file)

config = '../../pkjs/config.js'
with open(config) as conf_file:
    s = conf_file.readline()
    keys = []
    while (s): 
        suggestKey = re.search(r"messageKey\"\:(.[^,]*)", s)
        if suggestKey:
            keys.append(suggestKey.group(1).strip('" ,'))
        s = conf_file.readline()
    
def func(item):    
    return item.split('[',1)[0]

knownKeys = list(map(func, data["pebble"]["messageKeys"]))

for key in keys:
    #print('processing ', key);
    if key not in knownKeys:
        print('unknow key', key)



