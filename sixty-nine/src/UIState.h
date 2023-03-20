#pragma once

enum class Success
{
	FAIL,
	SUCCESS,
	ZERO
};

struct UIState
{
	Success RegistrationSuccess = Success::ZERO;
	Success CreateDMSuccess = Success::ZERO;
	uint32_t NewMessage = 0;
};