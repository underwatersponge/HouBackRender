import hou
import pprint
from BackRender import BackRen

data = BackRen.ReadJson("test.json")
pprint.pprint(data)

print("==========================")
hipFile = data["HipPath"]
renNodes = data["RenNodes"]
BackRen.StartRen(hipFile, renNodes)
