import sys
import hou
import pprint
from BackRender import GetRenNode
from BackRender import ReadStone

def GetNode(path):
    if path != None:
        hou.hipFile.load(path, ignore_load_warnings = True)
        renNode = GetRenNode.GetAllRenNode()
        return renNode

# path = r"C:\Users\HRXlubang\Desktop\ForTest\Ren\RenTest.hip"
# path = r"C:\Users\HRXlubang\Desktop\ForTest\ErrorWhenRen\ErrorWhenRenTest.hip"
# path = r"C:\Users\HRXlubang\Desktop\ForTest\Mess\MessTest.hip"

path = sys.argv[1]
nodes = GetNode(path)
if nodes != None:
    ReadStone.SaveJson(nodes)
print("Success search node and save!")
# pprint.pprint(nodes)

# print("=============test argv pass============")
# a = sys.argv[0]
# b = sys.argv[1]
# c = sys.argv[2]
# print(a)
# print(b)
# print(c)