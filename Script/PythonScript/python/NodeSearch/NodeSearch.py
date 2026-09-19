import hou
import nodesearch
import pprint
# from nodesearch import parser

def printInfo(node):
        type = node.type()
        typeCat = type.category()
        print("Type:", type)
        print("TypeCat:", typeCat)
        print("================================================")

def SearchNodes(network, typeName, typeCats, exact=False, recursive = False):
    result = []
    if hou.isUIAvailable():
        editor = hou.ui.paneTabOfType(hou.paneTabType.NetworkEditor)#maybe use latter

    for index,typeCat in enumerate(typeCats):
        matcher = nodesearch.NodeType(typeName, typeCat, exact)
        for node in matcher.nodes(network, recursive = recursive):
            result.append(node.path())

    return result


