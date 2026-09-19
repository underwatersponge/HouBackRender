import hou

from BackRender import GetRenNode
from BackRender import BackRen
from BackRender import ReadStone
import pprint

# GetCacheNode GetKarmaNode GetMantraNode

# TODO: want to sleep do not want test..........
'''
choices = ["mantra", "karma", "cache"]

selecteds = hou.ui.selectFromList(
    choices,
    title = "which to choose",
    message = "message",
    exclusive = False
)
if selecteds:
    print(selecteds)
'''
# ===================================== 
nodeToRen = GetRenNode.GetAllRenNode()
pprint.pprint(nodeToRen)

filepath = ReadStone.SaveJson(nodeToRen)
if True:
    data = BackRen.ReadJson(filepath)
    renlist = data["RenList"]
    for item in renlist:
        hipFile = item["HipPath"]
        renNodes = item["RenNode"]
        BackRen.StartRen(hipFile, renNodes)