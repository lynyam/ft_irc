#ifndef SIGNAL_HANDLER_HPP
# define SIGNAL_HANDLER_HPP

# include <signal.h>

extern volatile sig_atomic_t	g_stopSignal;

void	handleStopSignal(int signal);
void	setupSignals();

#endif