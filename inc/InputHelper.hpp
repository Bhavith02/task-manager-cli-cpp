#ifndef INPUTHELPER_HPP
#define INPUTHELPER_HPP

#include "Task.hpp"
#include <ctime>

class InputHelper {
public:
    static Priority selectPriority();
    static Status selectStatus();
    static time_t inputDueDate();
    static void setDueDateForTask(Task* task);
};

#endif // INPUTHELPER_HPP
