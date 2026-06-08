#include "SignalHandler.hpp"

volatile sig_atomic_t	g_stopSignal = 0;

void	handleStopSignal(int signal)
{
	(void)signal;
	g_stopSignal = 1;
}

void	setupSignals()
{
	struct sigaction	sa;

	sa.sa_handler = handleStopSignal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}