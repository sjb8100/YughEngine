#pragma once

#include <inttypes.h>
#include <steam/steam_api.h>
#include "dictionary.h" // Contains array.h as well
#include "object.h"
#include "reference.h"
#include "scene/resources/texture.h" // For avatars
#include "network.h"

class Steam : public Object, public NetSystem {
	GDCLASS(Steam, Object);

public:
	enum {
		TOP_LEFT = 0,
		TOP_RIGHT = 1,
		BOT_LEFT = 2,
		BOT_RIGHT = 3,
		ERR_NO_CLIENT = 2,
		ERR_NO_CONNECTION = 3,
		AUTH_SESSION_OK = 0,
		AUTH_SESSION_STEAM_NOT_CONNECTED = 1,
		AUTH_SESSION_NO_LICENSE = 2,
		AUTH_SESSION_VAC_BANNED = 3,
		AUTH_SESSION_LOGGED_IN_ELSEWHERE = 4,
		AUTH_SESSION_VAC_CHECK_TIMEOUT = 5,
		AUTH_SESSION_TICKET_CANCELED = 6,
		AUTH_SESSION_TICKET_ALREADY_USED = 7,
		AUTH_SESSION_TICKET_INVALID = 8,
		AUTH_SESSION_PUBLISHER_BANNED = 9,
		GLOBAL = 0,
		GLOBAL_AROUND_USER = 1,
		FRIENDS = 2,
		USERS = 3,
		LOBBY_KEY_LENGTH = 255,
		UGC_MAX_TITLE_CHARS = 128,
		UGC_MAX_DESC_CHARS = 8000,
		UGC_MAX_METADATA_CHARS = 5000,
		UGC_ITEM_COMMUNITY = 0,
		UGC_ITEM_MICROTRANSACTION = 1,
		UGC_ITEM_COLLECTION = 2,
		UGC_ITEM_ART = 3,
		UGC_ITEM_VIDEO = 4,
		UGC_ITEM_SCREENSHOT = 5,
		UGC_ITEM_GAME = 6,
		UGC_ITEM_SOFTWARE = 7,
		UGC_ITEM_CONCEPT = 8,
		UGC_ITEM_WEBGUIDE = 9,
		UGC_ITEM_INTEGRATEDGUIDE = 10,
		UGC_ITEM_MERCH = 11,
		UGC_ITEM_CONTROLLERBINDING = 12,
		UGC_ITEM_STEAMWORKSACCESSINVITE = 13,
		UGC_ITEM_STEAMVIDEO = 14,
		UGC_ITEM_GAMEMANAGEDITEM = 15,
		UGC_ITEM_MAX = 16,
		UGC_STATE_NONE = 0,
		UGC_STATE_SUBSCRIBED = 1,
		UGC_STATE_LEGACY = 2,
		UGC_STATE_INSTALLED = 4,
		UGC_STATE_UPDATE = 8,
		UGC_STATE_DOWNLOADING = 16,
		UGC_STATE_PENDING = 32,
		UGC_FILE_VISIBLE_PUBLIC = 0,
		UGC_FILE_VISIBLE_FRIENDS = 1,
		UGC_FILE_VISIBLE_PRIVATE = 2,
		STATUS_INVALID = 0,
		STATUS_PREPARING_CONFIG = 1,
		STATUS_PREPARING_CONTENT = 2,
		STATUS_UPLOADING_CONTENT = 3,
		STATUS_UPLOADING_PREVIEW = 4,
		STATUS_COMMITTING_CHANGES = 5,
		REMOTE_STORAGE_PLATFORM_NONE = 0,
		REMOTE_STORAGE_PLATFORM_WINDOWS = (1 << 0),
		REMOTE_STORAGE_PLATFORM_OSX = (1 << 1),
		REMOTE_STORAGE_PLATFORM_PS3 = (1 << 2),
		REMOTE_STORAGE_PLATFORM_LINUX = (1 << 3),
		REMOTE_STORAGE_PLATFORM_RESERVED2 = (1 << 4),
		REMOTE_STORAGE_PLATFORM_ALL = 0xffffffff
	};

	enum PersonaState {
		OFFLINE = 0, // Friend is not currently logged on.
		ONLINE = 1, // Friend is logged on.
		BUSY = 2, // User is on, but busy.
		AWAY = 3, // Auto-away feature.
		SNOOZE = 4, // Auto-away for a long time.
		LF_TRADE, // Online, trading.
		LF_PLAY, // Online, wanting to play.
		STATE_MAX,
		NOT_OFFLINE = 8,
		ALL = 9,
	};

	enum LobbyType {
		LOBBY_TYPE_PRIVATE = 0, // Only way to join the lobby is to invite to someone else.
		LOBBY_TYPE_FRIENDS_ONLY = 1, // Shows for friends or invitees, but not in lobby list.
		LOBBY_TYPE_PUBLIC = 2, // Shows for friends or invitees, but not in lobby list.
		LOBBY_TYPE_INVISIBLE = 3 // Returned by search, but not visible to other friends.
	};

	enum LobbyResponse {
		LOBBY_OK = 0, // Lobby was successfully created.
		LOBBY_NO_CONNECTION = 1, // Your Steam client doesn't have a connection to the back-end.
		LOBBY_TIMEOUT = 2, // Message to the Steam servers, but it didn't respond.
		LOBBY_FAIL = 3, // Server responded, but with an unknown internal error.
		LOBBY_ACCESS_DENIED = 4, // Game isn't set to allow lobbies, or your client does haven't rights to play the game.
		LOBBY_LIMIT_EXCEEDED = 5, // Game client has created too many lobbies.
	};

	// The square size of the Steam profile avatar
	enum AvatarSize {
		AVATAR_SMALL = 0, // 32 px
		AVATAR_MEDIUM = 1, // 64 px
		AVATAR_LARGE = 2, // 184 px
	};

	static Steam *get_singleton();
	Steam();
	~Steam();

	bool Initialize() override {
		return true;
	}

	// Holds an avatar Image for the game to pull from. Used because
	// we cannot figure out how to pass it back as a parameters.
	Ref<Image> AvatarImage;
	Ref<ImageTexture> AvatarImageTexture;

	// Returns a steam ID
	// More info here https://partner.steamgames.com/doc/api/steam_api
	// @param steamID - The user ID number
	// @param accountType - ENUM based on the Steam SDK EAccountType
	// @return The generated CSteamID
	CSteamID createSteamID(uint32_t steamID, int accountType = -1);

	// Steamworks ///////////////////////////////

	// Restart app through if it wasn't. If it was, it does nothing.
	// @param appID - The game's Steam appID
	// @return True if the game needs to restart
	bool restartAppIfNecessary(int appID);

	// Starts the steam API; call before you call anything else
	// @return True if the SDK succcessfuly started
	bool steamInit();

	// @return True if the Steam client is running
	bool isSteamRunning();

	// Apps /////////////////////////////////////

	// @return True if the active user is subscribed to the current App ID.
	bool isSubscribed();

	// @return True if the license owned by the user provides low violence depots.
	bool isLowViolence();

	// @return True if the current App ID is for Cyber Cafes.
	bool isCybercafe();

	// @return True if the user has a VAC ban on their account.
	bool isVACBanned();

	// Gets the current language that the user has set.
	// See here for full list of Steam API language codes.
	// https://partner.steamgames.com/doc/store/localization#supported_languages
	// @return The API language code the user has set
	String getCurrentGameLanguage();

	// Returns all the languages supported by the app
	// @see getCurrentGameLanguage()
	// @return A comma separated list of the languages the current app supports.
	String getAvailableGameLanguages();

	// Checks if the active user is subscribed to a specified AppId.
	// @param appID - The appID to check against
	// @return True if the user is subscribed to the app
	bool isSubscribedApp(int appID);

	// Checks if the user owns a specific DLC and if the DLC is installed
	// @param appID - The appID of the DLC to check against
	// @return True if the user owns the DLC and if the DLC is installed
	bool isDLCInstalled(int appID);

	// Gets the time of purchase of the specified app in Unix epoch format (time since Jan 1st, 1970).
	// Valve suggests to use this to reward people based on when they bought the game.
	// @param appID -  The appID of the app to check against
	// @return The purchase time in Unix epoch format (time since Jan 1st, 1970).
	int getEarliestPurchaseUnixTime(int appID);

	// Checks if the user is subscribed to the current app through a free weekend.
	// Suggested you contact Valve on how to package and secure your free weekend properly.
	// @return Returns false for users who have a retail or other type of license. True otherwise.
	bool isSubscribedFromFreeWeekend();

	// Get the number of DLC the user owns for a parent application/game.
	// @return Total number of DLC owned
	int getDLCCount();

	// Allows you to install an optional DLC.
	// @param appID - The appID of the DLC to install
	void installDLC(int appID);

	// Allows you to uninstall an optional DLC.
	// @param appID - The appID of the DLC to uninstall
	void uninstallDLC(int appID);

	// Gets the download progress for optional DLC.
	// @param appID - AppID of the DLC to check
	// @param bytesDownloaded - The number of bytes downloaded
	// @param bytesTotal - The total size of the download in bytes
	// @return - True if the specified DLC exists and is downloading
	bool getDLCDownloadProgress(int appID, uint64 *bytesDownloaded, uint64 *bytesTotal);

	// Checks if the user is running from a beta branch, and gets the name of the branch if they are.
	// @return The beta branch name
	String getCurrentBetaName();

	// Allows you to force verify game content on next launch.
	// @param missingFilesOnly - Only scan for missing files, don't verify the checksum of each file.
	// @return Not sure what this does? Possibly just a verify.
	// https://partner.steamgames.com/doc/api/ISteamApps
	bool markContentCorrupt(bool missingFilesOnly);

	// Gets the install folder for a specific AppID.
	// @param appID - The app to check
	// @return Path of the install folder for the appID game
	String getAppInstallDir(AppId_t appID);

	// Check if given application/game is installed, not necessarily owned.
	// @param appID - The appID of the app to check
	// @return True if the app is installed
	bool isAppInstalled(int appID);

	// Gets the Steam ID of the original owner of the current app.
	// If it's different from the current user then it is borrowed.
	// @return Steam ID of the true owner
	uint64_t getAppOwner();

	// Gets the associated launch parameter if the game is run
	// steam://run/<appid>/?param1=value1;param2=value2;param3=value3 etc.
	// @param key - The key as specified above
	// @return The launch parameter
	String getLaunchQueryParam(const String &key);
	
	// Return the build ID for this app; will change based on backend updates.
	// @return App build ID
	int getAppBuildId();

	//////////////////////////////////////////////////////////////////////////
	// Friends
	// Functions related to getting info about steam users, including the
	// user's friends and himself. Avatars, names, statuses, etc.
	// https://partner.steamgames.com/doc/api/ISteamFriends
	//////////////////////////////////////////////////////////////////////////

	// Get number of friends user has.
	// @return The number of friends
	int getFriendCount();

	// Get the user's Steam username.
	// @return The user's steam username
	String getPersonaName();

	// Get given friend's Steam username.
	// @param steamID - The steam ID of the user to get the name of
	// @return The user's name
	String getFriendPersonaName(int steamID);

	// Set the game information in Steam; used in 'View Game Info'
	// Rich presence data is automatically shared between friends in the same game.
	// Each user has a set of key/value pairs, up to 20 can be set.
	// Two important keys: status, connect.
	// "status" - A UTF-8 string that will show up in the 'view game info' dialog in the Steam friends list.
	// "connect" - A UTF-8 string that contains the command-line for how a friend can connect to a game.
	// This enables the 'join game' button in the 'view game info' dialog, in the steam friends list right
	// click menu, and on the players Steam community profile.
	// @param key - The key to set
	// @param value - The value associated with the key. If empty, deletes the key
	void setGameInfo(const String &key, const String &value);

	// Clear the game information in Steam; used in 'View Game Info'.
	void clearGameInfo();

	// Invite friend to current game/lobby.
	// The connect string is added to the command-line if the friend accepts
	// @param steamID - The ID of the friend
	// @param connectString - A string that lets the friend know how to join (like an IP address)
	void inviteFriend(int steamID, const String &connectString);

	// Set player as 'Played With' for game. Shown in "recently played" list
	// @param steamID - The ID of the person who was played with
	void setPlayedWith(int steamID);

	// Get list of players user has recently played game with, across all games
	// @return The list of players
	Array getRecentPlayers();

	// Get a player's avatar, a square inage
	// @param size - The size of the avatar to retrieve.
	// @param steamID - The steamID of the user to get. Only works for
	// users that the player knows about, like friends, leaderboard entries, etc
	void getFriendAvatar(int size = AVATAR_MEDIUM, int steamID = -1);

	// Get list of user's Steam groups; a mix of different Steamworks API group functions.
	// @return A dictionary of the user's groups
	Array getUserSteamGroups();

	// Get a list of user's Steam friends; a mix of different Steamworks API friend functions.
	// @return A dictionary of the user's friends
	Array getUserSteamFriends();

	// Activates the overlay with optional dialog to open the following:
	// "Friends", "Community", "Players", "Settings", "OfficialGameGroup",
	// "Stats", "Achievements", "LobbyInvite".
	// @param type - The overlay part to open
	void activateGameOverlay(const String &type);

	// Activates the overlay to the following:
	// "steamid", "chat", "jointrade", "stats", "achievements", "friendadd",
	// "friendremove", "friendrequestaccept", "friendrequestignore".
	// @param type - The string identifier to use
	// @param steamID - The ID of the user to open to
	void activateGameOverlayToUser(const String &type, int steamID);

	// Activates the overlay with specified web address.
	// @param url - The URL to open
	void activateGameOverlayToWebPage(const String &url);

	// Activates the overlay with the application/game Steam store page.
	// @param appID - The appID to load up
	void activateGameOverlayToStore(int appID = 0);

	// Activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
	// @param steamID - The user ID to invite
	void activateGameOverlayInviteDialog(int steamID);

	//////////////////////////////////////////////////////////////////////////
	/// MATCHMAKING
	// Used for creating matches on the Steam servers
	//////////////////////////////////////////////////////////////////////////

	// Create a lobby on the Steam servers, if private the lobby will
	// not be returned by any RequestLobbyList() call.
	// Notified with _lobby_created
	// @param lobbyType - The type of lobby to create
	// @param maxMembers - The max number of people who can join the lobby
	void createLobby(int lobbyType, int maxMembers);

	// Join an existing lobby.
	// Notified with _lobby_joined
	// @param steamIDLobby - The ID of the lobby to join
	void joinLobby(int steamIDLobby);

	// Leave a lobby, this will take effect immediately on the client side,
	// other users will be notified by LobbyChatUpdate_t callback.
	void leaveLobby(int steamIDLobby);

	// Invite another user to the lobby, the target user will receive a LobbyInvite_t callback
	// Notified with _lobby_invite
	// @return True if the invite is successfully sent, whether or not the target responds.
	bool inviteUserToLobby(int steamIDLobby, int steamIDInvitee);

	/////////////////////////////////////////////////
	///// REMOTE STORAGE ////////////////////////////
	// Used for cloud saving etc
	//////////////////////////////////////////////////////////////////////////

	// Write to given file from Steam Cloud.
	bool fileWrite(const String &file, const PoolByteArray &data, int32_t dataSize);

	// Read given file from Steam Cloud.
	Dictionary fileRead(const String &file, int32_t dataToRead);

	// Delete file from remote storage but leave it on local disk to remain accessible.
	bool fileForget(const String &file);

	// Delete a given file in Steam Cloud.
	bool fileDelete(const String &file);

	// Check if a given file exists in Steam Cloud.
	bool fileExists(const String &file);

	// Check if a given file is persisted in Steam Cloud.
	bool filePersisted(const String &file);

	// Get the size of a given file.
	int32_t getFileSize(const String &file);

	// Get the timestamp of when the file was uploaded/changed.
	int64_t getFileTimestamp(const String &file);

	// Gets the total number of local files synchronized by Steam Cloud.
	int32_t getFileCount();

	// Gets the file name and size of a file from the index.
	Dictionary getFileNameAndSize(int file);

	// Gets the number of bytes available, and used on the users Steam Cloud storage.
	Dictionary getQuota();

	// Obtains the platforms that the specified file will syncronize to.
	uint32_t getSyncPlatforms(const String &file);

	// Is Steam Cloud enabled on the user's account?
	bool isCloudEnabledForAccount();

	// Is Steam Cloud enabled for this application?
	bool isCloudEnabledForApp();

	// Set Steam Cloud enabled for this application.
	void setCloudEnabledForApp(bool enabled);

	/////////////////////////////////////////////////
	///// SCREENSHOTS ///////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	// Adds a screenshot to the user's Steam screenshot library from disk.
	uint32_t addScreenshotToLibrary(const String &filename, const String &thumbnailFilename, int width, int height);

	// Toggles whether the overlay handles screenshots.
	void hookScreenshots(bool hook);

	// Checks if the app is hooking screenshots.
	bool isScreenshotsHooked();

	// Sets optional metadata about a screenshot's location.
	bool setLocation(uint32_t screenshot, const String &location);

	// Causes Steam overlay to take a screenshot.
	void triggerScreenshot();

	// Writes a screenshot to the user's Steam screenshot library.
	uint32_t writeScreenshot(const PoolByteArray &RGB, int width, int height);

	/////////////////////////////////////////////////
	///// USERS /////////////////////////////////////
	// Functions for dealing with users. Authentication and so on.
	//////////////////////////////////////////////////////////////////////////

	// Get an authentication ticket.
	uint32_t getAuthSessionTicket();

	// Cancels an auth ticket.
	void cancelAuthTicket(uint32_t authTicket);

	// Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused.
	int beginAuthSession(uint32_t authTicket, uint64_t steamID);

	// Ends an auth session.
	void endAuthSession(uint64_t steamID);

	// Get user's Steam ID.
	uint64_t getSteamID();

	// Check, true/false, if user is logged into Steam currently.
	bool loggedOn();

	// Get the user's Steam level.
	int getPlayerSteamLevel();

	// Get the user's Steam installation path (this function is depreciated).
	String getUserDataFolder();

	// (LEGACY FUNCTION) Set data to be replicated to friends so that they can join your game.
	// If using the steam auth system, it automatically sets this, so isn't needed
	void advertiseGame(const String &serverIP, int port);

	// Trading Card badges data access, if you only have one set of cards, the series will be 1.
	// The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1).
	int getGameBadgeLevel(int series, bool foil);

	/////////////////////////////////////////////////
	///// USER STATS ////////////////////////////////
	// Used to set achievements and player stats and so on.
	/////////////////////////////////////////////////

	// Clears a given achievement.
	bool clearAchievement(const String &name);

	// Return true/false if user has given achievement.
	bool getAchievement(const String &name);

	// Returns the percentage of users who have unlocked the specified achievement.
	Dictionary getAchievementAchievedPercent(const String &name);

	// Get general attributes for an achievement
	String getAchievementDisplayAttribute(const String &name, const String &key);

	// Gets the icon for an achievement
	int getAchievementIcon(const String &name);

	// Gets the 'API name' for an achievement index
	String getAchievementName(uint32_t iAchievement);

	// Get the number of achievements.
	uint32_t getNumAchievements();

	//  Get the amount of players currently playing the current game (online + offline).
	void getNumberOfCurrentPlayers();

	// Get the value of a float statistic.
	float getStatFloat(const String &name);

	// Get the value of an integer statistic.
	int getStatInt(const String &name);

	// Reset all Steam statistics; optional to reset achievements.
	bool resetAllStats(bool achievementsToo = true);

	// Request all statistics and achievements from Steam servers.
	bool requestCurrentStats();

	// Asynchronously fetch the data for the percentages.
	void requestGlobalAchievementPercentages();

	// Set a given achievement.
	bool setAchievement(const String &name);

	// Set a float statistic.
	bool setStatFloat(const String &name, float value);

	// Set an integer statistic.
	bool setStatInt(const String &name, int value);

	// Store all statistics, and achievements, on Steam servers; must be called to "pop" achievements.
	bool storeStats();

	// Find a given leaderboard, by name.
	void findLeaderboard(const String &name);

	// Get the name of a leaderboard.
	String getLeaderboardName();

	// Get the total number of entries in a leaderboard, as of the last request.
	int getLeaderboardEntryCount();

	// Request all rows for friends of user.
	void downloadLeaderboardEntries(int start, int end, int type = GLOBAL);

	// Request a maximum of 100 users with only one outstanding call at a time.
	void downloadLeaderboardEntriesForUsers(Array usersID);

	// Upload a leaderboard score for the user.
	void uploadLeaderboardScore(int score, bool keepBest = false);

	// Once all entries are accessed, the data will be freed up and the handle will become invalid, use this to store it.
	void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t handle, int entryCount);

	// Get the currently used leaderboard handle.
	uint64_t getLeaderboardHandle();

	// Get the currently used leaderboard entries.
	Array getLeaderboardEntries();

	// Get the achievement status, and the time it was unlocked if unlocked (in seconds since January 1, 19).
	bool getAchievementAndUnlockTime(const String &name, bool achieved, uint32_t unlockTime);

	// Achievement progress, triggers an AchievementProgress callback, that is all.
	// Calling this with X out of X progress will NOT set the achievement, the game must still do that.
	bool indicateAchievementProgress(const String &name, int currentProgress, int maxProgress);

	/////////////////////////////////////////////////
	///// UTILS /////////////////////////////////////
	// Useful functions in general
	//////////////////////////////////////////////////////////////////////////

	// Get the user's country by IP.
	// @return The 2 digit ISO 3166-1-alpha-2 format country code which client is running in.
	String getIPCountry();

	// Returns true/false if Steam overlay is enabled.
	bool isOverlayEnabled();

	// Get the Steam user interface language.
	String getSteamUILanguage();

	// Get the Steam ID of the running application/game.
	int getAppID();

	// Gets the image bytes from an image handle.
	Dictionary getImageRGBA(int iImage);

	// Gets the size of a Steam image handle.
	Dictionary getImageSize(int iImage);

	// Return amount of time, in seconds, user has spent in this session.
	int getSecondsSinceAppActive();

	// Get the amount of battery power, clearly for laptops.
	int getCurrentBatteryPower();

	// Is Steam running in VR?
	bool isSteamRunningInVR();

	// Get the actual time.
	int getServerRealTime();

	// Returns true if Steam & the Steam Overlay are running in Big Picture mode.
	bool isSteamInBigPictureMode();

	// Ask SteamUI to create and render its OpenVR dashboard.
	void startVRDashboard();

	// Set the position where overlay shows notifications.
	void setOverlayNotificationPosition(int pos);

	//////////////////////////////////////////////////////////////////////////
	// Steap API Controller functions
	// These are used to improve the experience of players using the
	// steam cobntroller with your game
	//////////////////////////////////////////////////////////////////////////

	// Reconfigure the controller to use the specified action set.
	void activateActionSet(uint64_t controllerHandle, uint64_t actionSetHandle);

	// Lookup the handle for an Action Set.
	uint64_t getActionSetHandle(const String &actionSetName);

	// Returns the current state of the supplied analog game action.
	Dictionary getAnalogActionData(uint64_t controllerHandle, uint64_t analogActionHandle);

	// Get the handle of the specified Analog action.
	uint64_t getAnalogActionHandle(const String &actionName);

	// Get the origin(s) for an analog action within an action.
	Array getAnalogActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t analogActionHandle);

	// Get current controllers handles.
	Array getConnectedControllers();

	// Returns the associated controller handle for the specified emulated gamepad.
	uint64_t getControllerForGamepadIndex(int index);

	// Get the currently active action set for the specified controller.
	uint64_t getCurrentActionSet(uint64_t controllerHandle);

	// Returns the current state of the supplied digital game action.
	Dictionary getDigitalActionData(uint64_t controllerHandle, uint64_t digitalActionHandle);

	// Get the handle of the specified digital action.
	uint64_t getDigitalActionHandle(const String &actionName);

	// Get the origin(s) for an analog action within an action.
	Array getDigitalActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t digitalActionHandle);

	// Returns the associated gamepad index for the specified controller.
	int getGamepadIndexForController(uint64_t controllerHandle);

	// Returns raw motion data for the specified controller.
	Dictionary getMotionData(uint64_t controllerHandle);

	// TODO: These are all bad names. Redo them.
	// Start SteamControllers interface.
	bool init();

	// Syncronize controllers.
	void runFrame();

	// Invokes the Steam overlay and brings up the binding screen.
	bool showBindingPanel(uint64_t controllerHandle);

	// Stop SteamControllers interface.
	bool shutdown();

	// Trigger a vibration event on supported controllers.
	void triggerVibration(uint64_t controllerHandle, uint16_t leftSpeed, uint16_t rightSpeed);

	// Music ////////////////////////////////////

	// Is Steam music enabled.
	bool musicIsEnabled();

	// Is Steam music playing something.
	bool musicIsPlaying();

	// Get the volume level of the music.
	float musicGetVolume();

	// Pause whatever Steam music is playing.
	void musicPause();

	// Play current track/album.
	void musicPlay();

	// Play next track/album.
	void musicPlayNext();

	// Play previous track/album.
	void musicPlayPrev();

	// Set the volume of Steam music.
	void musicSetVolume(float value);

	/////////////////////////////////////////////////
	///// WORKSHOP //////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	// Download new or update already installed item. If returns true,
	// wait for DownloadItemResult_t. If item is already installed,
	// then files on disk should not be used until callback received.
	// If item is not subscribed to, it will be cached for some time.
	// If bHighPriority is set, any other item download will be suspended
	// and this item downloaded ASAP.
	bool downloadItem(int publishedFileID, bool highPriority);

	// SuspendDownloads( true ) will suspend all workshop downloads until SuspendDownloads( false ) is called or the game ends.
	void suspendDownloads(bool suspend);

	// Starts the item update process.
	uint64_t startItemUpdate(AppId_t appID, int fileId);

	// Gets the current state of a workshop item on this client.
	int getItemState(int publishedFileID);
	void createItem(AppId_t appID, int fileType);

	// Sets a new title for an item.
	bool setItemTitle(uint64_t updateHandle, const String &title);

	// Sets a new description for an item.
	bool setItemDescription(uint64_t updateHandle, const String &description);

	// Sets the language of the title and description that will be set in this item update.
	bool setItemUpdateLanguage(uint64_t updateHandle, const String &language);

	// Sets arbitrary metadata for an item. This metadata can be returned from queries without having to download and install the actual content.
	bool setItemMetadata(uint64_t updateHandle, const String &metadata);

	// Sets the visibility of an item.
	bool setItemVisibility(uint64_t updateHandle, int visibility);

	// Sets the folder that will be stored as the content for an item.
	bool setItemContent(uint64_t updateHandle, const String &contentFolder);

	// Sets the primary preview image for the item.
	bool setItemPreview(uint64_t updateHandle, const String &previewFile);

	// Uploads the changes made to an item to the Steam Workshop; to be called after setting your changes.
	void submitItemUpdate(uint64_t updateHandle, const String &changeNote);

protected:
	
	static Steam *singleton;

private:
	bool isInitSuccess;

	// Leaderboards
	SteamLeaderboard_t leaderboard_handle;
	Array leaderboard_entries;

	// Authentication
	struct TicketData {
		uint32_t id;
		uint32_t *buffer;
		uint32_t size;
	};
	Vector<TicketData> tickets;

	/////////////////////////////////////////////
	// STEAM CALLBACKS //////////////////////////
	// These are bound and returned when Steam finishes its job
	/////////////////////////////////////////////

	//// Apps callbacks

	// Posted after the user gains ownership of DLC & that DLC is installed.
	STEAM_CALLBACK(Steam, _dlc_installed, DlcInstalled_t);

	// Signal for file details acquired.
	STEAM_CALLBACK(Steam, _file_details_result, FileDetailsResult_t);

	//// Friends callbacks

	// Called when the avatar has been loaded and is ready to read
	STEAM_CALLBACK(Steam, _avatar_loaded, AvatarImageLoaded_t);

	//// Matchmaking callbacks

	// Signal the lobby has been created.
	STEAM_CALLBACK(Steam, _lobby_created, LobbyCreated_t);

	// Signal that lobby has been joined.
	STEAM_CALLBACK(Steam, _lobby_joined, LobbyEnter_t);

	// Signal that a lobby invite was sent.
	STEAM_CALLBACK(Steam, _lobby_invite, LobbyInvite_t);

	// Signal a game/lobby join has been requested.
	STEAM_CALLBACK(Steam, _join_requested, GameRichPresenceJoinRequested_t);

	// When connected to a server.
	STEAM_CALLBACK(Steam, _server_connected, SteamServersConnected_t);

	// When disconnected from a server.
	STEAM_CALLBACK(Steam, _server_disconnected, SteamServersDisconnected_t);

	// Screenshot callbacks
	// A screenshot has been requested by the user.
	STEAM_CALLBACK(Steam, _screenshot_ready, ScreenshotReady_t);

	//// User callbacks

	// Response from getAuthSessionTicket.
	STEAM_CALLBACK(Steam, _get_auth_session_ticket_response, GetAuthSessionTicketResponse_t);

	// Called when an auth ticket has been validated.
	STEAM_CALLBACK(Steam, _validate_auth_ticket_response, ValidateAuthTicketResponse_t);

	// User stat callbacks
	CCallResult<Steam, NumberOfCurrentPlayers_t> callResultNumberOfCurrentPlayers;
	void _number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure);

	// User stats are ready.
	STEAM_CALLBACK(Steam, _user_stats_received, UserStatsReceived_t);

	// Result of an achievement icon that has been fetched.
	STEAM_CALLBACK(Steam, _user_achievement_icon_fetched, UserAchievementIconFetched_t);

	// Signal a leaderboard entry has been uploaded.
	CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
	void _leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure);

	// Signal a leaderboard has been loaded or has failed.
	CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
	void _leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure);

	// Signal leaderboard entries are downloaded.
	CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
	void _leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);

	// Global achievements percentages are ready.
	CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
	void _global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure);

	//// Utility callbacks

	// Signal when overlay is triggered.
	STEAM_CALLBACK(Steam, _overlay_toggled, GameOverlayActivated_t);

	// Signal when battery power is running low, less than 10 minutes left.
	STEAM_CALLBACK(Steam, _low_power, LowBatteryPower_t);

	//// Workshop callbacks

	// Called when a workshop item has been installed or updated.
	STEAM_CALLBACK(Steam, _workshop_item_installed, ItemInstalled_t);

	// Result of a workshop item being created.
	CCallResult<Steam, CreateItemResult_t> callResultItemCreate;
	void _workshop_item_created(CreateItemResult_t *callData, bool bIOFailure);

	// Result of a workshop item being updated.
	CCallResult<Steam, SubmitItemUpdateResult_t> callResultItemUpdate;
	void _workshop_item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure);


	// Run the Steamworks API callbacks
	// Should be run every frame in order for the callbacks to function
	void run_callbacks() {
		SteamAPI_RunCallbacks();
	}

	void RunCallbacks() {
		SteamAPI_RunCallbacks();
	}

	void WriteAchievementProgress(String Name, float Progress) {
		// First sets the float, then sets the achievement if it's done
		setStatFloat(Name, Progress);

		if (Progress >= 100)
			setAchievement(Name);
	}

protected:
	//void Steam::_bind_methods()
	// Binds the methods to be used in GDScript
	static void _bind_methods()
	 {
		ClassDB::bind_method("restartAppIfNecessary", &Steam::restartAppIfNecessary);
		ClassDB::bind_method("steamInit", &Steam::steamInit);
		ClassDB::bind_method("isSteamRunning", &Steam::isSteamRunning);
		ClassDB::bind_method("run_callbacks", &Steam::run_callbacks);

		// Apps Bind Methods ////////////////////////
		ClassDB::bind_method("isSubscribed", &Steam::isSubscribed);
		ClassDB::bind_method("isLowViolence", &Steam::isLowViolence);
		ClassDB::bind_method("isCybercafe", &Steam::isCybercafe);
		ClassDB::bind_method("isVACBanned", &Steam::isVACBanned);
		ClassDB::bind_method("getCurrentGameLanguage", &Steam::getCurrentGameLanguage);
		ClassDB::bind_method("getAvailableGameLanguages", &Steam::getAvailableGameLanguages);
		ClassDB::bind_method("isSubscribedApp", &Steam::isSubscribedApp);
		ClassDB::bind_method("isDLCInstalled", &Steam::isDLCInstalled);
		ClassDB::bind_method("getEarliestPurchaseUnixTime", &Steam::getEarliestPurchaseUnixTime);
		ClassDB::bind_method("isSubscribedFromFreeWeekend", &Steam::isSubscribedFromFreeWeekend);
		ClassDB::bind_method("getDLCCount", &Steam::getDLCCount);
		ClassDB::bind_method("installDLC", &Steam::installDLC);
		ClassDB::bind_method("uninstallDLC", &Steam::uninstallDLC);
		ClassDB::bind_method("getCurrentBetaName", &Steam::getCurrentBetaName);
		ClassDB::bind_method("markContentCorrupt", &Steam::markContentCorrupt);
		ClassDB::bind_method("getAppInstallDir", &Steam::getAppInstallDir);
		ClassDB::bind_method("isAppInstalled", &Steam::isAppInstalled);
		ClassDB::bind_method("getAppOwner", &Steam::getAppOwner);
		ClassDB::bind_method("getLaunchQueryParam", &Steam::getLaunchQueryParam);
		ClassDB::bind_method("getAppBuildId", &Steam::getAppBuildId);

		// Controllers Bind Methods /////////////////
		ClassDB::bind_method("activateActionSet", &Steam::activateActionSet);
		ClassDB::bind_method("getActionSetHandle", &Steam::getActionSetHandle);
		ClassDB::bind_method("getAnalogActionData", &Steam::getAnalogActionData);
		ClassDB::bind_method("getAnalogActionHandle", &Steam::getAnalogActionHandle);
		ClassDB::bind_method("getAnalogActionOrigins", &Steam::getAnalogActionOrigins);
		ClassDB::bind_method("getConnectedControllers", &Steam::getConnectedControllers);
		ClassDB::bind_method("getControllerForGamepadIndex", &Steam::getControllerForGamepadIndex);
		ClassDB::bind_method("getCurrentActionSet", &Steam::getCurrentActionSet);
		ClassDB::bind_method("getDigitalActionData", &Steam::getDigitalActionData);
		ClassDB::bind_method("getDigitalActionHandle", &Steam::getDigitalActionHandle);
		ClassDB::bind_method("getDigitalActionOrigins", &Steam::getDigitalActionOrigins);
		ClassDB::bind_method("getMotionData", &Steam::getMotionData);
		ClassDB::bind_method("init", &Steam::init);
		ClassDB::bind_method("runFrame", &Steam::runFrame);
		ClassDB::bind_method("showBindingPanel", &Steam::showBindingPanel);
		ClassDB::bind_method("shutdown", &Steam::shutdown);
		ClassDB::bind_method("triggerVibration", &Steam::triggerVibration);

		// Friends Bind Methods /////////////////////
		ClassDB::bind_method("getFriendCount", &Steam::getFriendCount);
		ClassDB::bind_method("getPersonaName", &Steam::getPersonaName);
		ClassDB::bind_method("getFriendPersonaName", &Steam::getFriendPersonaName);
		ClassDB::bind_method(D_METHOD("setGameInfo", "key", "value"), &Steam::setGameInfo);
		ClassDB::bind_method(D_METHOD("clearGameInfo"), &Steam::clearGameInfo);
		ClassDB::bind_method(D_METHOD("inviteFriend", "steamID", "connect_string"), &Steam::inviteFriend);
		ClassDB::bind_method(D_METHOD("setPlayedWith", "steamID"), &Steam::setPlayedWith);
		ClassDB::bind_method("getRecentPlayers", &Steam::getRecentPlayers);
		ClassDB::bind_method(D_METHOD("getFriendAvatar", "size", "steamID"), &Steam::getFriendAvatar, DEFVAL(AVATAR_MEDIUM), DEFVAL(-1));
		ClassDB::bind_method("getUserSteamGroups", &Steam::getUserSteamGroups);
		ClassDB::bind_method("getUserSteamFriends", &Steam::getUserSteamFriends);
		ClassDB::bind_method(D_METHOD("activateGameOverlay", "type"), &Steam::activateGameOverlay, DEFVAL(""));
		ClassDB::bind_method(D_METHOD("activateGameOverlayToUser", "type", "steamID"), &Steam::activateGameOverlayToUser, DEFVAL(""), DEFVAL(0));
		ClassDB::bind_method(D_METHOD("activateGameOverlayToWebPage", "url"), &Steam::activateGameOverlayToWebPage);
		ClassDB::bind_method(D_METHOD("activateGameOverlayToStore", "appID"), &Steam::activateGameOverlayToStore, DEFVAL(0));
		ClassDB::bind_method(D_METHOD("activateGameOverlayInviteDialog", "steamID"), &Steam::activateGameOverlayInviteDialog);

		// Matchmaking Bind Methods /////////////////
		ClassDB::bind_method(D_METHOD("createLobby", "type"), &Steam::createLobby, DEFVAL(2));
		ClassDB::bind_method("joinLobby", &Steam::joinLobby);
		ClassDB::bind_method("leaveLobby", &Steam::leaveLobby);
		ClassDB::bind_method("inviteUserToLobby", &Steam::inviteUserToLobby);

		// Music Bind Methods ///////////////////////
		ClassDB::bind_method("musicIsEnabled", &Steam::musicIsEnabled);
		ClassDB::bind_method("musicIsPlaying", &Steam::musicIsPlaying);
		ClassDB::bind_method("musicGetVolume", &Steam::musicGetVolume);
		ClassDB::bind_method("musicPause", &Steam::musicPause);
		ClassDB::bind_method("musicPlay", &Steam::musicPlay);
		ClassDB::bind_method("musicPlayNext", &Steam::musicPlayNext);
		ClassDB::bind_method("musicPlayPrev", &Steam::musicPlayPrev);
		ClassDB::bind_method("musicSetVolume", &Steam::musicSetVolume);

		// Remote Storage Bind Methods //////////////
		ClassDB::bind_method("fileWrite", &Steam::fileWrite);
		ClassDB::bind_method("fileRead", &Steam::fileRead);
		ClassDB::bind_method("fileForget", &Steam::fileForget);
		ClassDB::bind_method("fileDelete", &Steam::fileDelete);
		ClassDB::bind_method("fileExists", &Steam::fileExists);
		ClassDB::bind_method("filePersisted", &Steam::filePersisted);
		ClassDB::bind_method("getFileSize", &Steam::getFileSize);
		ClassDB::bind_method("getFileTimestamp", &Steam::getFileTimestamp);
		ClassDB::bind_method("getFileCount", &Steam::getFileCount);
		ClassDB::bind_method("getFileNameAndSize", &Steam::getFileNameAndSize);
		ClassDB::bind_method("getQuota", &Steam::getQuota);
		ClassDB::bind_method("getSyncPlatforms", &Steam::getSyncPlatforms);
		ClassDB::bind_method("isCloudEnabledForAccount", &Steam::isCloudEnabledForAccount);
		ClassDB::bind_method("isCloudEnabledForApp", &Steam::isCloudEnabledForApp);
		ClassDB::bind_method("setCloudEnabledForApp", &Steam::setCloudEnabledForApp);

		// Screenshoot Bind Methods /////////////////
		ClassDB::bind_method("addScreenshotToLibrary", &Steam::addScreenshotToLibrary);
		ClassDB::bind_method("hookScreenshots", &Steam::hookScreenshots);
		ClassDB::bind_method("isScreenshotsHooked", &Steam::isScreenshotsHooked);
		ClassDB::bind_method("setLocation", &Steam::setLocation);
		ClassDB::bind_method("triggerScreenshot", &Steam::triggerScreenshot);
		ClassDB::bind_method("writeScreenshot", &Steam::writeScreenshot);

		// User Bind Methods ////////////////////////
		ClassDB::bind_method("getAuthSessionTicket", &Steam::getAuthSessionTicket);
		ClassDB::bind_method("cancelAuthTicket", &Steam::cancelAuthTicket);
		ClassDB::bind_method("beginAuthSession", &Steam::beginAuthSession);
		ClassDB::bind_method("endAuthSession", &Steam::endAuthSession);
		ClassDB::bind_method("getSteamID", &Steam::getSteamID);
		ClassDB::bind_method("loggedOn", &Steam::loggedOn);
		ClassDB::bind_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
		ClassDB::bind_method("getUserDataFolder", &Steam::getUserDataFolder);
		ClassDB::bind_method(D_METHOD("advertiseGame", "serverIP", "port"), &Steam::advertiseGame);
		ClassDB::bind_method("getGameBadgeLevel", &Steam::getGameBadgeLevel);

		// User Stats Bind Methods //////////////////
		ClassDB::bind_method("clearAchievement", &Steam::clearAchievement);
		ClassDB::bind_method("getAchievement", &Steam::getAchievement);
		ClassDB::bind_method("getAchievementAchievedPercent", &Steam::getAchievementAchievedPercent);
		ClassDB::bind_method(D_METHOD("getAchievementDisplayAttribute", "name", "key"), &Steam::getAchievementDisplayAttribute);
		ClassDB::bind_method(D_METHOD("getAchievementIcon", "name"), &Steam::getAchievementIcon);
		ClassDB::bind_method(D_METHOD("getAchievementName", "iAchievement"), &Steam::getAchievementName);
		ClassDB::bind_method("getNumAchievements", &Steam::getNumAchievements);
		ClassDB::bind_method("getNumberOfCurrentPlayers", &Steam::getNumberOfCurrentPlayers);
		ClassDB::bind_method("getStatFloat", &Steam::getStatFloat);
		ClassDB::bind_method("getStatInt", &Steam::getStatInt);
		ClassDB::bind_method("resetAllStats", &Steam::resetAllStats);
		ClassDB::bind_method("requestCurrentStats", &Steam::requestCurrentStats);
		ClassDB::bind_method("requestGlobalAchievementPercentages", &Steam::requestGlobalAchievementPercentages);
		ClassDB::bind_method("setAchievement", &Steam::setAchievement);
		ClassDB::bind_method("setStatFloat", &Steam::setStatFloat);
		ClassDB::bind_method("setStatInt", &Steam::setStatInt);
		ClassDB::bind_method("storeStats", &Steam::storeStats);
		ClassDB::bind_method(D_METHOD("findLeaderboard", "name"), &Steam::findLeaderboard);
		ClassDB::bind_method("getLeaderboardName", &Steam::getLeaderboardName);
		ClassDB::bind_method("getLeaderboardEntryCount", &Steam::getLeaderboardEntryCount);
		ClassDB::bind_method(D_METHOD("downloadLeaderboardEntries", "range_start", "range_end", "type"), &Steam::downloadLeaderboardEntries, DEFVAL(GLOBAL));
		ClassDB::bind_method(D_METHOD("downloadLeaderboardEntriesForUsers", "usersID"), &Steam::downloadLeaderboardEntriesForUsers);
		ClassDB::bind_method(D_METHOD("uploadLeaderboardScore", "score", "keep_best"), &Steam::uploadLeaderboardScore, DEFVAL(true));
		ClassDB::bind_method("getLeaderboardEntries", &Steam::getLeaderboardEntries);
		ClassDB::bind_method("getAchievementAndUnlockTime", &Steam::getAchievementAndUnlockTime);
		ClassDB::bind_method("indicateAchievementProgress", &Steam::indicateAchievementProgress);

		// Utils Bind Methods ///////////////////////
		ClassDB::bind_method("getIPCountry", &Steam::getIPCountry);
		ClassDB::bind_method("isOverlayEnabled", &Steam::isOverlayEnabled);
		ClassDB::bind_method("getSteamUILanguage", &Steam::getSteamUILanguage);
		ClassDB::bind_method("getAppID", &Steam::getAppID);
		ClassDB::bind_method(D_METHOD("getImageRGBA", "image"), &Steam::getImageRGBA);
		ClassDB::bind_method(D_METHOD("getImageSize", "image"), &Steam::getImageSize);
		ClassDB::bind_method("getSecondsSinceAppActive", &Steam::getSecondsSinceAppActive);
		ClassDB::bind_method(D_METHOD("setOverlayNotificationPosition", "pos"), &Steam::setOverlayNotificationPosition);
		ClassDB::bind_method("getCurrentBatteryPower", &Steam::getCurrentBatteryPower);
		ClassDB::bind_method("getServerRealTime", &Steam::getServerRealTime);
		ClassDB::bind_method("isSteamRunningInVR", &Steam::isSteamRunningInVR);
		ClassDB::bind_method("isSteamInBigPictureMode", &Steam::isSteamInBigPictureMode);
		ClassDB::bind_method("startVRDashboard", &Steam::startVRDashboard);

		// Workshop Bind Methods ////////////////////
		ClassDB::bind_method("downloadItem", &Steam::downloadItem);
		ClassDB::bind_method("suspendDownloads", &Steam::suspendDownloads);
		ClassDB::bind_method("startItemUpdate", &Steam::startItemUpdate);
		ClassDB::bind_method("getItemState", &Steam::getItemState);
		ClassDB::bind_method("createItem", &Steam::createItem);
		ClassDB::bind_method("setItemTitle", &Steam::setItemTitle);
		ClassDB::bind_method("setItemDescription", &Steam::setItemDescription);
		ClassDB::bind_method("setItemUpdateLanguage", &Steam::setItemUpdateLanguage);
		ClassDB::bind_method("setItemMetadata", &Steam::setItemMetadata);
		ClassDB::bind_method("setItemVisibility", &Steam::setItemVisibility);
		ClassDB::bind_method("setItemContent", &Steam::setItemContent);
		ClassDB::bind_method("setItemPreview", &Steam::setItemPreview);
		ClassDB::bind_method("submitItemUpdate", &Steam::submitItemUpdate);

		// Signals //////////////////////////////////
		ADD_SIGNAL(MethodInfo("file_details_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "fileSize"), PropertyInfo(Variant::INT, "fileHash"), PropertyInfo(Variant::INT, "flags")));
		ADD_SIGNAL(MethodInfo("join_requested", PropertyInfo(Variant::INT, "from"), PropertyInfo(Variant::STRING, "connect_string")));
		ADD_SIGNAL(MethodInfo("avatar_loaded", PropertyInfo(Variant::INT, "size")));
		ADD_SIGNAL(MethodInfo("number_of_current_players", PropertyInfo(Variant::BOOL, "success"), PropertyInfo(Variant::INT, "players")));
		ADD_SIGNAL(MethodInfo("leaderboard_loaded", PropertyInfo(Variant::INT, "leaderboard"), PropertyInfo(Variant::INT, "found")));
		ADD_SIGNAL(MethodInfo("leaderboard_uploaded", PropertyInfo(Variant::BOOL, "success"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::BOOL, "score_changed"), PropertyInfo(Variant::INT, "global_rank_new"), PropertyInfo(Variant::INT, "global_rank_previous")));
		ADD_SIGNAL(MethodInfo("leaderboard_entries_loaded"));
		ADD_SIGNAL(MethodInfo("overlay_toggled", PropertyInfo(Variant::BOOL, "active")));
		ADD_SIGNAL(MethodInfo("low_power", PropertyInfo(Variant::INT, "power")));
		ADD_SIGNAL(MethodInfo("lobby_created", PropertyInfo(Variant::INT, "lobby")));
		ADD_SIGNAL(MethodInfo("lobby_joined", PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "permissions"), PropertyInfo(Variant::BOOL, "locked"), PropertyInfo(Variant::INT, "response")));
		ADD_SIGNAL(MethodInfo("lobby_invite", PropertyInfo(Variant::INT, "inviter"), PropertyInfo(Variant::INT, "lobby"), PropertyInfo(Variant::INT, "game")));
		ADD_SIGNAL(MethodInfo("connection_changed", PropertyInfo(Variant::BOOL, "connected")));
		ADD_SIGNAL(MethodInfo("dlc_installed", PropertyInfo(Variant::INT, "app")));
		ADD_SIGNAL(MethodInfo("get_auth_session_ticket_response", PropertyInfo(Variant::INT, "ticket"), PropertyInfo(Variant::INT, "result")));
		ADD_SIGNAL(MethodInfo("validate_auth_ticket_response", PropertyInfo(Variant::INT, "steamID"), PropertyInfo(Variant::INT, "auth_session_reponse"), PropertyInfo(Variant::INT, "owner_steamID")));
		ADD_SIGNAL(MethodInfo("screenshot_ready", PropertyInfo(Variant::INT, "screenshot_handle"), PropertyInfo(Variant::INT, "result")));
		ADD_SIGNAL(MethodInfo("user_stats_received", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::INT, "userID")));
		ADD_SIGNAL(MethodInfo("user_achievement_icon_fetched", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::STRING, "achievementName"), PropertyInfo(Variant::BOOL, "achieved"), PropertyInfo(Variant::INT, "iconHandle")));
		ADD_SIGNAL(MethodInfo("global_achievement_percentages_ready", PropertyInfo(Variant::INT, "gameID"), PropertyInfo(Variant::INT, "result")));
		ADD_SIGNAL(MethodInfo("workshop_item_created"));
		ADD_SIGNAL(MethodInfo("workshop_item_updated"));
		ADD_SIGNAL(MethodInfo("workshop_item_installed"));

		//////////////////////////////////////////////////////////////////////////
		// Binding constants
		//////////////////////////////////////////////////////////////////////////

		BIND_CONSTANT(OFFLINE); // 0
		BIND_CONSTANT(ONLINE); // 1
		BIND_CONSTANT(BUSY); // 2
		BIND_CONSTANT(AWAY); // 3
		BIND_CONSTANT(SNOOZE); // 4
		BIND_CONSTANT(LF_TRADE); // 5
		BIND_CONSTANT(LF_PLAY); // 6
		BIND_CONSTANT(NOT_OFFLINE); // Custom
		BIND_CONSTANT(ALL); // Custom
		// Initialization errors ////////////////////
		BIND_CONSTANT(ERR_NO_CLIENT);
		BIND_CONSTANT(ERR_NO_CONNECTION);
		// Authentication responses /////////////////
		BIND_CONSTANT(AUTH_SESSION_OK);
		BIND_CONSTANT(AUTH_SESSION_STEAM_NOT_CONNECTED);
		BIND_CONSTANT(AUTH_SESSION_NO_LICENSE);
		BIND_CONSTANT(AUTH_SESSION_VAC_BANNED);
		BIND_CONSTANT(AUTH_SESSION_LOGGED_IN_ELSEWHERE);
		BIND_CONSTANT(AUTH_SESSION_VAC_CHECK_TIMEOUT);
		BIND_CONSTANT(AUTH_SESSION_TICKET_CANCELED);
		BIND_CONSTANT(AUTH_SESSION_TICKET_ALREADY_USED);
		BIND_CONSTANT(AUTH_SESSION_TICKET_INVALID);
		BIND_CONSTANT(AUTH_SESSION_PUBLISHER_BANNED);
		// Avatar sizes /////////////////////////////
		BIND_CONSTANT(AVATAR_SMALL);
		BIND_CONSTANT(AVATAR_MEDIUM);
		BIND_CONSTANT(AVATAR_LARGE);
		// Overlay notification locations ///////////
		BIND_CONSTANT(TOP_LEFT);
		BIND_CONSTANT(TOP_RIGHT);
		BIND_CONSTANT(BOT_LEFT);
		BIND_CONSTANT(BOT_RIGHT);
		// Matchmaking types ////////////////////////
		BIND_CONSTANT(LOBBY_TYPE_PRIVATE);
		BIND_CONSTANT(LOBBY_TYPE_FRIENDS_ONLY);
		BIND_CONSTANT(LOBBY_TYPE_PUBLIC);
		BIND_CONSTANT(LOBBY_TYPE_INVISIBLE);
		BIND_CONSTANT(LOBBY_KEY_LENGTH); // Maximum number of characters a lobby metadata key can be.
		// Matchmaking lobby responses //////////////
		BIND_CONSTANT(LOBBY_OK); // Lobby was successfully created.
		BIND_CONSTANT(LOBBY_NO_CONNECTION); // Your Steam client doesn't have a connection to the back-end.
		BIND_CONSTANT(LOBBY_TIMEOUT); // Message to the Steam servers, but it didn't respond.
		BIND_CONSTANT(LOBBY_FAIL); // Server responded, but with an unknown internal error.
		BIND_CONSTANT(LOBBY_ACCESS_DENIED); // Game isn't set to allow lobbies, or your client does haven't rights to play the game.
		BIND_CONSTANT(LOBBY_LIMIT_EXCEEDED); // Game client has created too many lobbies.
		// Remote Storage
		BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_NONE);
		BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_WINDOWS);
		BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_OSX);
		BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_PS3);
		BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_LINUX);
		BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_RESERVED2);
		BIND_CONSTANT(REMOTE_STORAGE_PLATFORM_ALL);
		// Workshop item characters /////////////////
		BIND_CONSTANT(UGC_MAX_TITLE_CHARS); // 128
		BIND_CONSTANT(UGC_MAX_DESC_CHARS); // 8000
		BIND_CONSTANT(UGC_MAX_METADATA_CHARS); // 5000
		// Workshop item types //////////////////////
		BIND_CONSTANT(UGC_ITEM_COMMUNITY); // Normal items that can be subscribed to.
		BIND_CONSTANT(UGC_ITEM_MICROTRANSACTION); // Item that is meant to be voted on for the purpose of selling in-game.
		BIND_CONSTANT(UGC_ITEM_COLLECTION); // A collection of Workshop items.
		BIND_CONSTANT(UGC_ITEM_ART); // Artwork.
		BIND_CONSTANT(UGC_ITEM_VIDEO); // External video.
		BIND_CONSTANT(UGC_ITEM_SCREENSHOT); // Screenshot.
		BIND_CONSTANT(UGC_ITEM_GAME); // Unused, used to be for Greenlight game entries
		BIND_CONSTANT(UGC_ITEM_SOFTWARE); // Unused, used to be for Greenlight software entries.
		BIND_CONSTANT(UGC_ITEM_CONCEPT); // Unused, used to be for Greenlight concepts.
		BIND_CONSTANT(UGC_ITEM_WEBGUIDE); // Steam web guide.
		BIND_CONSTANT(UGC_ITEM_INTEGRATEDGUIDE); // Application integrated guide.
		BIND_CONSTANT(UGC_ITEM_MERCH); // Workshop merchandise meant to be voted on for the purpose of being sold.
		BIND_CONSTANT(UGC_ITEM_CONTROLLERBINDING); // Steam Controller bindings.
		BIND_CONSTANT(UGC_ITEM_STEAMWORKSACCESSINVITE); // Only used internally in Steam.
		BIND_CONSTANT(UGC_ITEM_STEAMVIDEO); // Steam video.
		BIND_CONSTANT(UGC_ITEM_GAMEMANAGEDITEM); // Managed completely by the game, not the user, and not shown on the web.
		BIND_CONSTANT(UGC_ITEM_MAX); // Only used for enumerating.
		// Workshop item states //////////////////////
		BIND_CONSTANT(UGC_STATE_NONE); // Not tracked on client.
		BIND_CONSTANT(UGC_STATE_SUBSCRIBED); // Current user is subscribed to this item, not just cached.
		BIND_CONSTANT(UGC_STATE_LEGACY); // Was created with ISteamRemoteStorage.
		BIND_CONSTANT(UGC_STATE_INSTALLED); // Is installed and usable (but maybe out of date).
		BIND_CONSTANT(UGC_STATE_UPDATE); // Needs an update, either because it's not installed yet or creator updated content.
		BIND_CONSTANT(UGC_STATE_DOWNLOADING); // Update is currently downloading.
		BIND_CONSTANT(UGC_STATE_PENDING); // DownloadItem() was called for this item, content isn't available until DownloadItemResult_t is fired.
		// Workshop item visibility//////////////////
		BIND_CONSTANT(UGC_FILE_VISIBLE_PUBLIC);
		BIND_CONSTANT(UGC_FILE_VISIBLE_FRIENDS);
		BIND_CONSTANT(UGC_FILE_VISIBLE_PRIVATE);
		// Workshop item update status //////////////
		BIND_CONSTANT(STATUS_INVALID); // Update handle was invalid, job might be finished, listen to SubmitItemUpdateResult_t.
		BIND_CONSTANT(STATUS_PREPARING_CONFIG); // Update is processing configuration data.
		BIND_CONSTANT(STATUS_PREPARING_CONTENT); // Update is reading and processing content files.
		BIND_CONSTANT(STATUS_UPLOADING_CONTENT); // Update is uploading content changes to Steam.
		BIND_CONSTANT(STATUS_UPLOADING_PREVIEW); // Update is uploading new preview file image.
		BIND_CONSTANT(STATUS_COMMITTING_CHANGES); // Update is committing all changes.
	}
};
