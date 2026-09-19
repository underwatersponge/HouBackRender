import hou
import pprint
from BackRender import BackRen

data = BackRen.ReadJson("RenList.json")
pprint.pprint(data)
print("==========================")
renlist = data["RenList"]
for item in renlist:
    hipFile = item["HipPath"]
    renNodes = item["RenNode"]
    BackRen.StartRen(hipFile, renNodes)