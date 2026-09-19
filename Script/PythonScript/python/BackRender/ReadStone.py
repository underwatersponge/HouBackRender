import hou
import json

def SaveJson(nodeToRen):
    hipPath = hou.hipFile.path()
    data = {}
    data["RenList"] = []
    item = {}
    item["HipPath"] = hipPath
    item["RenNode"] = nodeToRen
    data["RenList"].append(item)
    fileToSave = ""
    if hou.isUIAvailable():
        fileToSave = hou.ui.selectFile(start_directory = hou.getenv("HIP"),file_type = hou.fileType.Any, pattern = ".json", title = "Choose a file")
    else:
        temp = hipPath.split("/")
        del temp[-1]
        tempstr = "/".join(temp)
        fileToSave = tempstr + "/savedNode.json"
    if fileToSave:
        try:
            with open(fileToSave, "w", encoding="utf-8") as f:
                json.dump(data, f, ensure_ascii = False, indent = 4)
        except Exception as e:
            errorStr = "SaveJson failed:{}".format(e)
            print(errorStr)
            if hou.isUIAvailable():
                hou.ui.displayMessage(errorStr)
    return fileToSave
