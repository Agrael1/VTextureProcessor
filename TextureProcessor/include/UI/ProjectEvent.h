#pragma once
#include <filesystem>

struct ProjectEvent: public QEvent {
    std::filesystem::path projPath;

    ProjectEvent(std::filesystem::path projPath)
        :projPath(projPath), QEvent((QEvent::Type)(QEvent::User+1)) {}
};

