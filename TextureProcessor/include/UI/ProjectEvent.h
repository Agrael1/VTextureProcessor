/**
 * @file ProjectEvent.h
 * @author David Černý (xcerny74)
 * @brief Custom event for application
 */
#pragma once
#include <filesystem>

struct ProjectEvent: public QEvent {
    std::filesystem::path projPath;

    ProjectEvent(std::filesystem::path&& projPath)
        :projPath(std::move(projPath)), QEvent((QEvent::Type)(QEvent::User+1)) {}
};

