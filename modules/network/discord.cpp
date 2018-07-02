#include "discord.h"

Discord *Discord::singleton = nullptr;

Discord::Discord() {
	singleton = this;
	memset(&DiscordPresence, 0, sizeof(DiscordPresence));
}

Discord::~Discord() {

}
