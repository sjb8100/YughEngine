#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <inttypes.h>
#include <steam/steam_api.h>

#include "dictionary.h" // Contains array.h as well
#include "object.h"
#include "reference.h"
#include "scene/resources/texture.h" // For avatars

class Steam : public Object {
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

	// TODO: Remove this. Only temporary.
	// Holds an avatar Image for the game to pull from. Used because
	// we cannot figure out how to pass it back as a parameters.
	Ref<Image> TemporaryAvatar;

	Ref<ImageTexture> tempf;

	// Simple get for TemporaryAvatar
	Ref<ImageTexture> GetLoadedAvatar() const;

	CSteamID createSteamID(uint32_t steamID, int accountType = -1);

	// Creates an Image from an array of bytes
	// @param size - the width of the image
	// @param buffer - the image byte array
	Image drawAvatar(int size, uint8 *buffer);

	// Steamworks ///////////////////////////////

	// Restart app
	bool restartAppIfNecessary(int value);
	bool steamInit();
	bool isSteamRunning();

	// Apps /////////////////////////////////////
	bool isSubscribed();
	bool isLowViolence();
	bool isCybercafe();
	bool isVACBanned();
	String getCurrentGameLanguage();
	String getAvailableGameLanguages();
	bool isSubscribedApp(int value);
	bool isDLCInstalled(int value);
	int getEarliestPurchaseUnixTime(int value);
	bool isSubscribedFromFreeWeekend();
	int getDLCCount();
	Array getDLCDataByIndex();
	void installDLC(int value);
	void uninstallDLC(int value);
	String getCurrentBetaName();
	bool markContentCorrupt(bool missingFilesOnly);
	//	uint32_t getInstalledDepots(int appID, uint32* depots, uint32 maxDepots);
	String getAppInstallDir(AppId_t appID);
	bool isAppInstalled(int value);
	uint64_t getAppOwner();
	String getLaunchQueryParam(const String &key);
	//	bool getDLCDownloadProgress(int appID, uint64* bytesDownloaded, uint64* bytesTotal);
	int getAppBuildId();
	void getFileDetails(const String &filename);

	// Controller ///////////////////////////////
	void activateActionSet(uint64_t controllerHandle, uint64_t actionSetHandle);
	uint64_t getActionSetHandle(const String &actionSetName);
	Dictionary getAnalogActionData(uint64_t controllerHandle, uint64_t analogActionHandle);
	uint64_t getAnalogActionHandle(const String &actionName);
	Array getAnalogActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t analogActionHandle);
	Array getConnectedControllers();
	uint64_t getControllerForGamepadIndex(int index);
	uint64_t getCurrentActionSet(uint64_t controllerHandle);
	Dictionary getDigitalActionData(uint64_t controllerHandle, uint64_t digitalActionHandle);
	uint64_t getDigitalActionHandle(const String &actionName);
	Array getDigitalActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t digitalActionHandle);
	int getGamepadIndexForController(uint64_t controllerHandle);
	Dictionary getMotionData(uint64_t controllerHandle);
	bool init();
	void runFrame();
	bool showBindingPanel(uint64_t controllerHandle);
	bool shutdown();
	void triggerVibration(uint64_t controllerHandle, uint16_t leftSpeed, uint16_t rightSpeed);

	// Friends //////////////////////////////////
	int getFriendCount();
	String getPersonaName();
	String getFriendPersonaName(int steamID);
	void setGameInfo(const String &key, const String &value);
	void clearGameInfo();
	void inviteFriend(int id, const String &connectString);
	void setPlayedWith(int steamID);
	Array getRecentPlayers();
	void getFriendAvatar(int size = AVATAR_MEDIUM);
	Array getUserSteamGroups();
	Array getUserSteamFriends();
	void activateGameOverlay(const String &type);
	void activateGameOverlayToUser(const String &type, int steamID);
	void activateGameOverlayToWebPage(const String &url);
	void activateGameOverlayToStore(int appID = 0);
	void activateGameOverlayInviteDialog(int steamID);

	// Matchmaking //////////////////////////////
	void createLobby(int lobbyType, int maxMembers);
	void joinLobby(int steamIDLobby);
	void leaveLobby(int steamIDLobby);
	bool inviteUserToLobby(int steamIDLobby, int steamIDInvitee);

	// Music ////////////////////////////////////
	bool musicIsEnabled();
	bool musicIsPlaying();
	float musicGetVolume();
	void musicPause();
	void musicPlay();
	void musicPlayNext();
	void musicPlayPrev();
	void musicSetVolume(float value);

	// Remote Storage ///////////////////////////
	bool fileWrite(const String &file, const PoolByteArray &data, int32_t dataSize);
	Dictionary fileRead(const String &file, int32_t dataToRead);
	bool fileForget(const String &file);
	bool fileDelete(const String &file);
	bool fileExists(const String &file);
	bool filePersisted(const String &file);
	int32_t getFileSize(const String &file);
	int64_t getFileTimestamp(const String &file);
	int32_t getFileCount();
	Dictionary getFileNameAndSize(int file);
	Dictionary getQuota();
	uint32_t getSyncPlatforms(const String &file);
	bool isCloudEnabledForAccount();
	bool isCloudEnabledForApp();
	void setCloudEnabledForApp(bool enabled);

	// Screenshots //////////////////////////////
	uint32_t addScreenshotToLibrary(const String &filename, const String &thumbnailFilename, int width, int height);
	void hookScreenshots(bool hook);
	bool isScreenshotsHooked();
	bool setLocation(uint32_t screenshot, const String &location);
	void triggerScreenshot();
	uint32_t writeScreenshot(const PoolByteArray &RGB, int width, int height);

	// Users ////////////////////////////////////
	uint32_t getAuthSessionTicket();
	void cancelAuthTicket(uint32_t authTicket);
	int beginAuthSession(uint32_t authTicket, uint64_t steamID);
	void endAuthSession(uint64_t steamID);
	uint64_t getSteamID();
	bool loggedOn();
	int getPlayerSteamLevel();
	String getUserDataFolder();
	void advertiseGame(const String &serverIP, int port);
	int getGameBadgeLevel(int series, bool foil);

	// User Stats ///////////////////////////////
	bool clearAchievement(const String &name);
	uint32_t getNumAchievements();
	void getNumberOfCurrentPlayers();
	bool getAchievement(const String &name);
	Dictionary getAchievementAchievedPercent(const String &name);
	String getAchievementDisplayAttribute(const String &name, const String &key);
	int getAchievementIcon(const String &name);
	String getAchievementName(uint32_t iAchievement);
	float getStatFloat(const String &name);
	int getStatInt(const String &name);
	bool resetAllStats(bool achievementsToo = true);
	bool requestCurrentStats();
	void requestGlobalAchievementPercentages();
	bool setAchievement(const String &name);
	bool setStatFloat(const String &name, float value);
	bool setStatInt(const String &name, int value);
	bool storeStats();
	void findLeaderboard(const String &name);
	String getLeaderboardName();
	int getLeaderboardEntryCount();
	void downloadLeaderboardEntries(int start, int end, int type = GLOBAL);
	void downloadLeaderboardEntriesForUsers(Array usersID);
	void uploadLeaderboardScore(int score, bool keepBest = false);
	void getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t handle, int entryCount);
	uint64_t getLeaderboardHandle();
	Array getLeaderboardEntries();
	bool getAchievementAndUnlockTime(const String &name, bool achieved, uint32_t unlockTime);
	bool indicateAchievementProgress(const String &name, int currentProgress, int maxProgress);

	// Utils ////////////////////////////////////
	String getIPCountry();
	bool isOverlayEnabled();
	String getSteamUILanguage();
	int getAppID();
	Dictionary getImageRGBA(int iImage);
	Dictionary getImageSize(int iImage);
	int getSecondsSinceAppActive();
	void setOverlayNotificationPosition(int pos);
	int getCurrentBatteryPower();
	bool isSteamRunningInVR();
	int getServerRealTime();
	bool isSteamInBigPictureMode();
	void startVRDashboard();

	// Workshop /////////////////////////////////
	bool downloadItem(int publishedFileID, bool highPriority);
	void suspendDownloads(bool suspend);
	uint64_t startItemUpdate(AppId_t appID, int fileId);
	int getItemState(int publishedFileID);
	//	int getItemUpdateProgress(uint64_t handle, uint64_t *bytesProcessed, uint64_t* bytesTotal);
	void createItem(AppId_t appID, int fileType);
	bool setItemTitle(uint64_t updateHandle, const String &title);
	bool setItemDescription(uint64_t updateHandle, const String &description);
	bool setItemUpdateLanguage(uint64_t updateHandle, const String &language);
	bool setItemMetadata(uint64_t updateHandle, const String &metadata);
	bool setItemVisibility(uint64_t updateHandle, int visibility);
	//	bool setItemTags(uint64_t updateHandle, const PoolStringArray ** stringArray, const int32 stringCount);
	bool setItemContent(uint64_t updateHandle, const String &contentFolder);
	bool setItemPreview(uint64_t updateHandle, const String &previewFile);
	void submitItemUpdate(uint64_t updateHandle, const String &changeNote);
	//	Array getSubscribedItems();
	//	bool getItemInstallInfo(int fileID, uint64_t *sizeOnDisk, char *folder, uint32_t folderSize, uint32_t *timeStamp);
	//	bool getItemDownloadInfo(int fileID, uint64_t *bytesDownloaded, uint64_t *bytesTotal);

protected:
	static void _bind_methods();
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
	//
	// Apps callbacks
	STEAM_CALLBACK(Steam, _dlc_installed, DlcInstalled_t);
	STEAM_CALLBACK(Steam, _file_details_result, FileDetailsResult_t);
	//	CCallResult<Steam, FileDetailsResult_t> callResultFileDetails;
	//	void _file_details_result(FileDetailsResult_t *callData);
	// Friends callbacks
	STEAM_CALLBACK(Steam, _avatar_loaded, AvatarImageLoaded_t);
	// Matchmaking callbacks
	STEAM_CALLBACK(Steam, _lobby_created, LobbyCreated_t);
	STEAM_CALLBACK(Steam, _lobby_joined, LobbyEnter_t);
	STEAM_CALLBACK(Steam, _lobby_invite, LobbyInvite_t);
	STEAM_CALLBACK(Steam, _join_requested, GameRichPresenceJoinRequested_t);
	STEAM_CALLBACK(Steam, _server_connected, SteamServersConnected_t);
	STEAM_CALLBACK(Steam, _server_disconnected, SteamServersDisconnected_t);
	// Screenshot callbacks
	STEAM_CALLBACK(Steam, _screenshot_ready, ScreenshotReady_t);
	// User callbacks
	STEAM_CALLBACK(Steam, _get_auth_session_ticket_response, GetAuthSessionTicketResponse_t);
	STEAM_CALLBACK(Steam, _validate_auth_ticket_response, ValidateAuthTicketResponse_t);
	// User stat callbacks
	CCallResult<Steam, NumberOfCurrentPlayers_t> callResultNumberOfCurrentPlayers;
	void _number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure);
	STEAM_CALLBACK(Steam, _user_stats_received, UserStatsReceived_t);
	STEAM_CALLBACK(Steam, _user_achievement_icon_fetched, UserAchievementIconFetched_t);
	CCallResult<Steam, LeaderboardScoreUploaded_t> callResultUploadScore;
	void _leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure);
	CCallResult<Steam, LeaderboardFindResult_t> callResultFindLeaderboard;
	void _leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure);
	CCallResult<Steam, LeaderboardScoresDownloaded_t> callResultEntries;
	void _leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure);
	CCallResult<Steam, GlobalAchievementPercentagesReady_t> callResultGlobalAchievementPercentagesReady;
	void _global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure);
	// Utility callbacks
	STEAM_CALLBACK(Steam, _overlay_toggled, GameOverlayActivated_t);
	STEAM_CALLBACK(Steam, _low_power, LowBatteryPower_t);
	// Workshop callbacks
	STEAM_CALLBACK(Steam, _workshop_item_installed, ItemInstalled_t);
	CCallResult<Steam, CreateItemResult_t> callResultItemCreate;
	void _workshop_item_created(CreateItemResult_t *callData, bool bIOFailure);
	CCallResult<Steam, SubmitItemUpdateResult_t> callResultItemUpdate;
	void _workshop_item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure);
	// Run the Steamworks API callbacks
	void run_callbacks() {
		SteamAPI_RunCallbacks();
	}
};
#endif // GODOTSTEAM_H
