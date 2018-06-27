#pragma once

#include "dictionary.h"
#include "discord_rpc.h"
#include "object.h"
#include "reference.h"

class Discord : public Object {
	GDCLASS(Discord, Object);

public:
	static Discord *get_singleton() { return singleton; }

	Discord() {
		singleton = this;
		memset(&DiscordPresence, 0, sizeof(DiscordPresence));

		DiscordReadyHandler = &Discord::HandleDiscordReady;
		/*	DiscordErrorHandler = &HandleDiscordError;
		DiscordDisconnectedHandler = &HandleDiscordDisconnected;
		DiscordJoinGameHandler = &HandleDiscordJoinGame;
		DiscordSpectateGameHandler = &HandleDiscordSpectateGame;
		DiscordJoinRequestHandler = &HandleDiscordJoinRequest;*/
	}

	~Discord() {
	}

	// Call this to initialize Discord integration. Must be called before anything else is.
	void InitDiscord() {
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));

		void (*df)(const DiscordUser *c) = &Discord::HandleDiscordReady;

		handlers.ready = df;
		//handlers.errored = DiscordErrorHandler;
		//handlers.disconnected = DiscordDisconnectedHandler;
		//handlers.joinGame = DiscordJoinGameHandler;
		//handlers.spectateGame = DiscordSpectateGameHandler;
		//handlers.joinRequest = DiscordJoinRequestHandler;

		// Params are the discord ID, our DiscordEventHandlers, bAutoregister, and an optional steam appID
		Discord_Initialize("461414686882005002", &handlers, 1, nullptr);
	}

	// The user's current discord presence
	DiscordRichPresence DiscordPresence;

	

	// Simply sends on the current DiscordPresence to Discord
	void SendRichPresence(DiscordRichPresence NewPresence) {
		DiscordRichPresence SendPresence = NewPresence;
		Discord_UpdatePresence(&SendPresence);
	}

	// @param State - The user's current party status
	void SetPresenceState(String State) {
		DiscordPresence.state = State.utf8().get_data();
		SendRichPresence(DiscordPresence);
	}

	// @param Details - What the user is currently doing
	void SetPresenceDetails(String Details) {
		DiscordPresence.details = Details.utf8().get_data();
		SendRichPresence(DiscordPresence);
	}

	void (*DiscordReadyHandler)(const DiscordUser *c);
	void HandleDiscordReady(const DiscordUser *connectedUser) {
		emit_signal("discord_ready");
	}

	/*void (*DiscordErrorHandler)(int errcode, const char *message);
	void HandleDiscordError(int errcode, const char *message) {
		emit_signal("discord_error", errcode, Str(message));
	}

	void (*DiscordDisconnectedHandler)(int errcode, const char *message);
	void HandleDiscordDisconnected(int errcode, const char *message) {
		emit_signal("discord_disconnected", errcode);
	}

	void (*DiscordJoinGameHandler)(const char *secret);
	void HandleDiscordJoinGame(const char *secret) {
	}

	void (*DiscordSpectateGameHandler)(const char *secret);
	void HandleDiscordSpectateGame(const char *secret) {
	}

	void (*DiscordJoinRequestHandler)(const DiscordUser *request);
	void HandleDiscordJoinRequest(const DiscordUser *request) {
		emit_signal("discord_join_request");
	}*/

protected:
	static Discord *singleton;

	static void _bind_methods()
	{
		ClassDB::bind_method("InitDiscord", &Discord::InitDiscord);
		ClassDB::bind_method("SetPresenceState", &Discord::SetPresenceState);
		ClassDB::bind_method("SetPresenceDetails", &Discord::SetPresenceDetails);

		ADD_SIGNAL(MethodInfo("discord_ready"));
		ADD_SIGNAL(MethodInfo("discord_error", PropertyInfo(Variant::INT, "errcode"), PropertyInfo(Variant::STRING, "message")));
		ADD_SIGNAL(MethodInfo("discord_disconnected", PropertyInfo(Variant::INT, "errcode"), PropertyInfo(Variant::STRING, "message")));
		ADD_SIGNAL(MethodInfo("discord_join_game", PropertyInfo(Variant::STRING, "secret")));
		ADD_SIGNAL(MethodInfo("discord_spectate_game", PropertyInfo(Variant::STRING, "secret")));
		ADD_SIGNAL(MethodInfo("discord_join_request"));
	}
};

