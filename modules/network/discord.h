#pragma once

#include "dictionary.h"
#include "discord_rpc.h"
#include "network.h"
#include "object.h"
#include "reference.h"

//////////////////////////////////////////////////////////////////////////
// Discord integration for the Godot engine 3.0+
// https://discordapp.com/developers/docs/rich-presence/how-to
//////////////////////////////////////////////////////////////////////////

class Discord : public Object, public NetSystem {
	GDCLASS(Discord, Object);

public:
	static Discord *get_singleton() { return singleton; }

	Discord();

	~Discord();

	bool Initialize() override {
		return true;
	}

	// Call this to initialize Discord integration. Must be called before anything else is.
	void InitDiscord() {
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));

		handlers.ready = &HandleDiscordReady;
		handlers.errored = &HandleDiscordError;
		handlers.disconnected = &HandleDiscordDisconnected;
		handlers.joinGame = &HandleDiscordJoinGame;
		handlers.spectateGame = &HandleDiscordSpectateGame;
		handlers.joinRequest = &HandleDiscordJoinRequest;

		// Params are the discord ID, our DiscordEventHandlers, bAutoregister, and an optional steam appID
		// Replace the ID with your own game's ID
		Discord_Initialize("461414686882005002", &handlers, 1, nullptr);
	}

	// The user's current discord presence
	DiscordRichPresence DiscordPresence;

	String DiscordState;
	String DiscordDetails;
	String JoinSecret;
	String SpectateSecret;

	// Simply sends on the current DiscordPresence to Discord
	void SendRichPresence() {
		DiscordRichPresence SendPresence{};

		SendPresence.state = DiscordState.utf8().get_data();
		SendPresence.details = DiscordDetails.utf8().get_data();
		SendPresence.joinSecret = JoinSecret.utf8().get_data();
		SendPresence.spectateSecret = SpectateSecret.utf8().get_data();

		Discord_UpdatePresence(&SendPresence);
	}

	// @param State - The user's current party status
	void SetPresenceState(String State) {
		DiscordState = State;
		SendRichPresence();
	}

	// @param Details - What the user is currently doing
	void SetPresenceDetails(String Details) {
		DiscordDetails = Details;
		SendRichPresence();
	}

	void SetJoinSecret(String Secret) {
		JoinSecret = Secret;
		SendRichPresence();
	}

	void SetSpectateSecret(String Secret) {
		SpectateSecret = Secret;
		SendRichPresence();
	}

	// Called when Discord connects and is ready
	// @param connectedUser - The user discord the game is connected to
	void DiscordReady(const DiscordUser *connectedUser) {
		emit_signal("discord_ready");
	}

	static void HandleDiscordReady(const DiscordUser *connectedUser) {
		get_singleton()->DiscordReady(connectedUser);
	}

	// Called when Discord makes an error
	// Can't find documentation on the errcodes or messages.
	// @param errcode - Code of the discord error
	// @param message - String description of the error
	void DiscordError(int errcode, String message) {
		emit_signal("discord_error", errcode, message);
	}

	static void HandleDiscordError(int errcode, const char *message) {
		get_singleton()->DiscordError(errcode, String(message));
	}

	// Called when Discord disconnects
	// @param errcode - Code of the discord disconnect
	// @param message - Description of the disconnect
	void DiscordDisconnect(int errcode, String message) {
		emit_signal("discord_disconnect", errcode, message);
	}

	static void HandleDiscordDisconnected(int errcode, const char *message) {
		get_singleton()->DiscordDisconnect(errcode, String(message));
	}

	/////////////////////////////////////////////
	// Handles for joining and spectating
	// For more information, see here
	// https://discordapp.com/developers/docs/rich-presence/how-to
	/////////////////////////////////////////////

	// When a discord user selects to join a game, this is called
	// on their machine once the game has opened and connected to discord
	// @param secret - Use to join the appropriate game
	void DiscordJoinGame(String secret) {
		emit_signal("discord_join_game", secret);
	}

	static void HandleDiscordJoinGame(const char *secret) {
		get_singleton()->DiscordJoinGame(String(secret));
	}

	// When a user selects Spectate, their game opens and then this is called
	// @param secret - Sent from the game they select to spectate. Use to
	// connect to the correct game.
	void DiscordSpectateGame(String secret) {
		emit_signal("discord_spectate_game", secret);
	}

	static void HandleDiscordSpectateGame(const char *secret) {
		get_singleton()->DiscordSpectateGame(String(secret));
	}

	// Called when somebody asks to join your game
	// Should be handled by issuing a response
	// @see JoinResponse
	// @param request - The join request payload, with info about the
	// user asking to join
	void DiscordJoinRequest(const DiscordUser *request) {
		emit_signal("discord_join_request");
	}

	static void HandleDiscordJoinRequest(const DiscordUser *request) {
		get_singleton()->DiscordJoinRequest(request);
	}

	// Call this to respond to a JoinRequest
	// @param userID - The discord ID of the requestor
	// @param response - The DISCORD_REPLY of the selected choice
	void JoinResponse(String userID, int response) {
		Discord_Respond(userID.utf8().get_data(), response);
	}

	//////////////////////////////////////////////////////////////////////////
	// NetSystem implementation
	//////////////////////////////////////////////////////////////////////////

	// This must be called in order to sync with the Discord server.
	// Should be run as often as possible to keep up to date.
	void RunCallbacks() {
		Discord_RunCallbacks();
	}

	void WriteAchievementProgress(String Name, float Progress) {
	}

protected:
	static Discord *singleton;

	// Binding all methods and signals
	static void _bind_methods() {
		ClassDB::bind_method("InitDiscord", &Discord::InitDiscord);
		ClassDB::bind_method("SetPresenceState", &Discord::SetPresenceState);
		ClassDB::bind_method("SetPresenceDetails", &Discord::SetPresenceDetails);
		ClassDB::bind_method("SetSpectateSecret", &Discord::SetSpectateSecret);
		ClassDB::bind_method("SetJoinSecret", &Discord::SetJoinSecret);
		ClassDB::bind_method("JoinResponse", &Discord::JoinResponse);
		ClassDB::bind_method("RunCallbacks", &Discord::RunCallbacks);

		ADD_SIGNAL(MethodInfo("discord_ready"));
		ADD_SIGNAL(MethodInfo("discord_error", PropertyInfo(Variant::INT, "errcode"), PropertyInfo(Variant::STRING, "message")));
		ADD_SIGNAL(MethodInfo("discord_disconnected", PropertyInfo(Variant::INT, "errcode"), PropertyInfo(Variant::STRING, "message")));
		ADD_SIGNAL(MethodInfo("discord_join_game", PropertyInfo(Variant::STRING, "secret")));
		ADD_SIGNAL(MethodInfo("discord_spectate_game", PropertyInfo(Variant::STRING, "secret")));
		ADD_SIGNAL(MethodInfo("discord_join_request"));

		// Bindings for the constants defined by Discord
		BIND_CONSTANT(DISCORD_REPLY_NO);
		BIND_CONSTANT(DISCORD_REPLY_YES);
		BIND_CONSTANT(DISCORD_REPLY_IGNORE);
	}
};
