// Task Manager CLI v2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <json.hpp>
#include <fstream>


using json = nlohmann::json;

enum class STATUS
{
    todo, in_progress, done
};

class Task
{
public:
    int id; // Task id which is going to be assigned by Task_List Class
    std::string description; // Task description
    STATUS status; // Status
    std::string created_at; // timestamp created at
    std::string updated_at;// everytime a task is updated

    //Constructor 
    Task(int i, std::string d, const std::string ts, std::string updated_ts) :id{ i }, description{ d }, status{ STATUS::todo }, created_at{ ts }, updated_at{updated_ts} {}
    Task(int i, std::string d,STATUS status,const std::string ts, std::string updated_ts) :id{ i }, description{ d }, status{ status }, created_at{ ts }, updated_at{ updated_ts } {}
    
};

std::string capture_timestamp()
{// using ctime we capture a timestamp when the function is called
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now); // Fills local 'timeinfo' struct
    //All date values are converted to a string using to_string()
    std::string year = std::to_string(timeinfo.tm_year + 1900); // year
    std::string month = std::to_string(timeinfo.tm_mon + 1); // month
    std::string monthDay = std::to_string(timeinfo.tm_mday); // day
    std::string hour = std::to_string(timeinfo.tm_hour); // hour (military time)
    std::string minutes = std::to_string(timeinfo.tm_min); //minutes
    std::string seconds = std::to_string(timeinfo.tm_sec); // seconds
    std::string date = year + "-" + month + "-" + monthDay;
    std::string time = hour + ":" + minutes + ":" + seconds;
    
    return date + " " + time; // return 2026-04-09 09:17:45
}

class TaskList
{
private:
    //Helper functions
    void increment_task_id();//increments counter for task ids
    int find_task(int id); //returns index in vector, -1 if not found
    std::string status_to_string(STATUS status);//converts enum to string
    STATUS string_to_status(std::string); //converts string to status
   
public:
    int task_id = 1;
    std::vector<Task> tasks;
    //member functions
    void add_task(std::string description);
    void delete_task(int id);
    void update_task(int id, std::string desciption);
    void mark_status(int id, STATUS status);
    void list_tasks(STATUS filter, bool filterActive);
    //JSON Functions
    void load_from_json();
    void save_to_json();
};

void TaskList::add_task(std::string description)
{
    tasks.push_back(Task(task_id, description, capture_timestamp(), capture_timestamp()));
    std::cout << "Task created ID:{" << task_id << "} ";
    increment_task_id(); //increment task ID
    

    //Add to JSON file
    save_to_json(); 
}

int TaskList::find_task(int id)
{
  //return task index
    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        if (tasks[i].id == id)
            return i;

    }

    return -1;
}

std::string TaskList::status_to_string(STATUS status)
{
    switch (status)
    {
    case STATUS::todo:
        return "todo";
    case STATUS::in_progress:
        return "in-progress";
    case STATUS::done:  
    return "done";    
    default:
        return "invalid status";
    }
}

STATUS TaskList::string_to_status(std::string status)
{
    if (status == "in-progress")
        return STATUS::in_progress;

    if (status == "todo")
        return STATUS::todo;
    if (status == "done")
        return STATUS::done;
    
    std::cout << "Invalid status";
    return STATUS::todo;
}

void TaskList::increment_task_id()
{
    task_id++;
}

void TaskList::save_to_json()
{//save to json file
    
    json j;//json object
    json j_taskList_array = json::array();//create json array

    //iterate over tasksList and create json objects
    for (int i = 0; i < static_cast<int>(tasks.size()); i++) 
    {
        j["id"] = tasks[i].id;
        j["description"] = tasks[i].description;
        j["status"] = status_to_string(tasks[i].status);
        j["createdAt"] = tasks[i].created_at;
        j["updatedAt"] = tasks[i].updated_at;

       
        j_taskList_array.push_back(j);     
    }

    std::ofstream output_file("config.json");
    output_file << j_taskList_array.dump(4);
    output_file.close();

}

void TaskList::load_from_json()
{
    std::ifstream file;

    file.open("config.json");

    if (file.is_open())
    {
        json j_array = json::parse(file);
        //std::cout << j_array << '\n';
        
        for (int i = 0; i < static_cast<int>(j_array.size()); i++)
        {
            Task task = Task(j_array[i]["id"].get<int>(), j_array[i]["description"].get<std::string>(), 
                string_to_status(j_array[i]["status"].get<std::string>()), j_array[i]["createdAt"].get<std::string>(), j_array[i]["updatedAt"].get<std::string>());

            tasks.push_back(task);
        
        }

        if (!tasks.empty())
            task_id = tasks.back().id + 1;
        
    }
    else
    {
        
    }
}

void TaskList::delete_task(int id)
{
    int index = find_task(id);

    if (index == -1)
    {
        std::cout << "Task not found";
        return;
    }
    std::cout << "Task created ID:{" << id << "} ";
    tasks.erase(tasks.begin() + static_cast<size_t>(index));

    save_to_json();
}

void TaskList::update_task(int id, std::string description)
{//Update description & updated at timestamp
    int index = find_task(id);

    if (index == -1)
    {
        std::cout << "Task not found";
        return;
    }

    tasks.at(index).description = description;
    tasks.at(index).updated_at = capture_timestamp();
    save_to_json();
}

void TaskList::mark_status(int id, STATUS status)
{
    int index = find_task(id);

    if (index == -1) // Check if item exist 
    {
        std::cout << "Task not found";
        return; //Return error
    }

    tasks.at(index).status = status;
    tasks.at(index).updated_at = capture_timestamp(); //Update timestamp
    save_to_json();

}

void TaskList::list_tasks(STATUS filter, bool filterActive)
{ // Function to list task

    int counter = 0;
    //Check if tasks list is empty
    if (static_cast<int>(tasks.size()) < 1)
        std::cout << "No Tasks Found";

    // iterate through Tasks and print
    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        if (!filterActive) //check if filteractive is False
        {
            std::cout << "[" << tasks[i].id << "] " << tasks[i].description << " - " << TaskList::status_to_string(tasks[i].status)
                << " " << "(created: " << tasks[i].created_at << ")\n";
            
        }
        else
        {
            if (tasks[i].status == filter && static_cast<int>(tasks.size()) >= 1)
            { //Print task with filter applied
                std::cout << "[" << tasks[i].id << "] " << tasks[i].description << " - " << TaskList::status_to_string(tasks[i].status)
                    << " " << "(created: " << tasks[i].created_at << ")\n";
                counter++;
            }
           
        }

    }

    if (filterActive && counter == 0)
    {
      std::cout << "No tasks with status:" << TaskList::status_to_string(filter);

    }
        
}







int main(int argc, char* argv[])
{
    TaskList taskList;
    
    taskList.load_from_json();
    
    

    //std::cout << "arc:\t"<< argc;
    //for (int i = 0; i < argc; i++)
    //    std::cout << "argv[" << i << "] = " << argv[i] << '\n';

   

    if (argc < 2) // if syntax is not valid print error
    {
        std::cout << "SYNTAX ERROR: Please enter a valid command\n";
        return 1;
    }

    std::string command = argv[1];
  

    if (command == "add") //Add Task
    {
        
        if (argc < 3)
        {
            std::cout << "SYNTAX ERROR: add requires a description\n";
            return 1;    
        }
       taskList.add_task(std::string(argv[2]));
      
        
           
    }

    if (command == "delete")  //Delete item
    {
        
        if (argc >= 3)
            try
            {
                taskList.delete_task(std::stoi(argv[2]));
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << "\n";
            }     
    }


    if (command == "update")  //Update item
    {
       
        if (argc >= 4)
            try
            {
                taskList.update_task(std::stoi(argv[2]), argv[3]);
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << "\n";
            }
    }


    if (command == "list")// List command
    {
       
        if (argc == 2) // List all task
        {
            taskList.list_tasks(STATUS::todo, false);
        }
        else
        {
            std::string arg2 = argv[2];
            if (argc == 3 && arg2 == "done") // List done Task
            {
                taskList.list_tasks(STATUS::done, true);
            }
            else if (argc == 3 && arg2 == "todo") //List task todo
            {
                taskList.list_tasks(STATUS::todo, true);
            }
            else if (argc == 3 && arg2 == "in-progress") //list in-progress tasks
            {
                taskList.list_tasks(STATUS::in_progress, true);
            }
            else
            {
                std::cout << "INVALID FILTER: Please use { done, in-progress, todo }. \n";
            }

        }
      
    }
    //Marking a task as in-progress or done
    if (command == "mark-in-progress")
    {
        if (argc < 3)
        {
            std::cout << "SYNTAX ERROR: Invalid ID or Status\n";
            return 1;
        }
        taskList.mark_status(std::stoi(std::string(argv[2])), STATUS::in_progress);
        std::cout << std::string(argv[2]) << " marked in-progress\n";
    }
    
   

    if (command == "mark-done")
    {//mark task with status "done"
        if (argc < 3)
        {
            std::cout << "SYNTAX ERROR: Invalid ID or Status\n";
            return 1;
        }
        taskList.mark_status(std::stoi(std::string(argv[2])), STATUS::done);
        std::cout << std::string(argv[2]) << " marked done\n";
    }
   



   
   
}

