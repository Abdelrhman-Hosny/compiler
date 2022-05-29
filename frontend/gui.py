import PySimpleGUI as sg
import subprocess
import os
sg.theme("Dark Blue")  # Keep things interesting for your users


file_list_column = [
    [
        sg.Text("Folder"),
        sg.In(key="folder_path", enable_events=True, size=(30, 1)),
        sg.FolderBrowse(key="folder_browse"),
    ],
    [sg.Listbox(values=[], enable_events=True, size=(50, 20), key="file_list")],
]

file_viewer_column = [
    [sg.Text("File name: ", size=(70, 3), key="file_name")],
    [sg.Multiline(size=(70, 40), key="file_content")],
    [ 
        sg.Button("Save", key="save_file", enable_events=True, size=(30, 1)) ,
        sg.Button("Compile", key="compile_file", enable_events=True, size=(30, 1))
    ],
]


layout = [
    [
        sg.Column(file_list_column),
        sg.VerticalSeparator(),
        sg.Column(file_viewer_column),
    ]
]

window = sg.Window("Compiler Text Editor", layout)

while True:  # The Event Loop
    event, values = window.read()
    print(event, values)
    if event == sg.WIN_CLOSED or event == "Exit":
        break

    elif event == "folder_path":
        folder_path = values["folder_path"]
        print(folder_path)
        try:
            files = os.listdir(folder_path)
        except:
            files = []

        files = [f for f in files if f.endswith(".txt") or f.endswith(".py") or f.endswith(".c") or f.endswith(".cpp") or f.endswith(".md")]
        print(files)
        window["file_list"].update(files)

    elif event == "file_list":
        file_name = values["file_list"][0]
        print(file_name)
        file_path = os.path.join(folder_path, file_name)
        print(file_path)
        with open(file_path, "r") as f:
            file_content = f.read()
        window["file_name"].update(file_name)
        window["file_content"].update(file_content)
    
    elif event == "save_file" and len(values["file_list"]) > 0:
        
        file_name = values["file_list"][0]
        file_path = os.path.join(folder_path, file_name)
        file_content = values["file_content"]
        with open(file_path, "w") as f:
            f.write(file_content)

    elif event == "compile_file" and len(values["file_list"]) > 0:

        print(f"Current working directory : {os.getcwd()}")
        compiler_path = os.path.join(os.getcwd(), "build/a.out")
        print(f"Compiler path: {compiler_path}")
        file_name = values["file_list"][0]
        file_path = os.path.join(folder_path, file_name)
        file_content = values["file_content"]
        result = subprocess.Popen([compiler_path, file_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        stdout, stderr = result.communicate()
        print(f"std out {stdout.decode('utf-8')},\n stderr {stderr}")



window.close()
