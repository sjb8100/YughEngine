#include "network.h"

Network *Network::singleton = nullptr;

void Network::_notification(int p_what) {

	switch (p_what) {
		// TODO: Add a bool to check if we should run callbacks atm
		case NOTIFICATION_PROCESS: {
			RunCallbacks();
		}
	}
}
