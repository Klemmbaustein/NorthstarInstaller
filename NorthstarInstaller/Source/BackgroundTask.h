#pragma once
#include <atomic>
#include <string>
#include <thread>
#include <vector>

struct BackgroundTask
{
	size_t Type = 0;
	std::atomic<float> Progress = 0;
	std::string Status;
	std::thread* Thread = nullptr;

	// Will point to the task that is being executed right now.
	thread_local static BackgroundTask* ThisThreadPtr;

	BackgroundTask(void (*Function)());
	~BackgroundTask();

	static void SetProgress(float Progress);
	static void SetStatus(std::string NewStatus);

	static std::string CurrentTaskStatus;
	static float CurrentTaskProgress;
	static bool IsRunningTask;

	static void UpdateTaskStatus();

private:
	static std::vector<BackgroundTask*> AllTasks;
	static void TaskRun(void (*Function)(), BackgroundTask* Task);
};