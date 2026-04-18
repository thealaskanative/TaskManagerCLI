# TaskManager CLI

A command-line task manager built in C++ that lets you add, update, delete, and track tasks with persistent JSON storage. Built as a side project while working through *Programming: Principles and Practice Using C++* by Bjarne Stroustrup.

## Prerequisites

- Windows OS
- Visual Studio
- [nlohmann/json](https://github.com/nlohmann/json) header (`json.hpp`) — place in your project's include directory

## Running the Program

Open a terminal and navigate to the output folder:
```
cd "path\to\TaskManagerCLI\x64\Debug"
```

---

## Commands

### Add a Task
```
task-cli.exe add "Task description"
```
**Example:**
```
task-cli.exe add "Buy groceries"
```
Output: `Task created ID:{1}`

---

### List Tasks

**List all tasks:**
```
task-cli.exe list
```

**List by status:**
```
task-cli.exe list todo
task-cli.exe list in-progress
task-cli.exe list done
```

---

### Update a Task
```
task-cli.exe update <id> "New description"
```
**Example:**
```
task-cli.exe update 1 "Buy groceries and cook dinner"
```

---

### Mark Task Status
```
task-cli.exe mark-in-progress <id>
task-cli.exe mark-done <id>
```
**Example:**
```
task-cli.exe mark-in-progress 1
task-cli.exe mark-done 1
```

---

### Delete a Task
```
task-cli.exe delete <id>
```
**Example:**
```
task-cli.exe delete 1
```

---

## Storing Data

Tasks are saved automatically to `config.json` in the same directory as the executable. The file is created on first use and updated after every command.

**Example JSON structure:**
```json
[
    {
        "id": 1,
        "description": "Buy groceries",
        "status": "todo",
        "createdAt": "2026-4-11 15:48:33",
        "updatedAt": "2026-4-11 15:48:33"
    }
]
```
