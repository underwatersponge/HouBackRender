import hou
import pprint
from BackRender import BackRen

data = BackRen.ReadJson("RenList.json")
pprint.pprint(data)
print("==========================")
renlist = data["RenList"]
for index, item in enumerate(renlist):
    hipFile = item["HipPath"]
    print("processing hipfile:{current}/{total}".format(current = index + 1, total = len(renlist)), hipFile)
    renNodes = item["RenNode"]
    BackRen.StartRen(hipFile, renNodes)