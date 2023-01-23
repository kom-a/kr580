#pragma once

struct Settings
{
public:
	static Settings& Get();

private:
	Settings() = default;
	~Settings() = default;

public:
	Settings(const Settings&) = delete;
	void operator=(const Settings&) = delete;

public:
	int LoadAddress = 0x0800;
	bool LoadAtProgramCounter = false;
};