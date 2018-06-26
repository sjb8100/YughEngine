#include "godotsteam.h"
#include <steam/steam_api.h>

#include "core/io/ip.h"
#include "core/io/ip_address.h"

Steam *Steam::singleton = NULL;

Steam::Steam() {
	isInitSuccess = false;
	singleton = this;
	tickets.clear();
}

Steam *Steam::get_singleton() {
	return singleton;
}

CSteamID Steam::createSteamID(uint32_t steamID, int accountType) {
	CSteamID cSteamID;
	if (accountType < 0 || accountType >= k_EAccountTypeMax) {
		accountType = 1;
	}
	cSteamID.Set(steamID, EUniverse(k_EUniversePublic), EAccountType(accountType));
	return cSteamID;
}
/////////////////////////////////////////////////
///// STEAMWORKS FUNCTIONS //////////////////////
//
// Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't.
bool Steam::restartAppIfNecessary(int value) {
	return SteamAPI_RestartAppIfNecessary((AppId_t)value);
}
// Initialize Steamworks
bool Steam::steamInit() {
	return SteamAPI_Init();
	//printf("Godot Steam initialing...\n");
	isInitSuccess = SteamAPI_Init();
	int err = FAILED;
	if (isInitSuccess) {
		err = OK;
	}
	if (!SteamAPI_IsSteamRunning()) {
		err = ERR_NO_CLIENT;
	} else if (!SteamUser()->BLoggedOn()) {
		err = ERR_NO_CONNECTION;
	}
	if (err == OK && SteamUserStats() != NULL) {
		// Load stats and achievements automatically.
		SteamUserStats()->RequestCurrentStats();
	}
	return err;
}
// Returns true/false if Steam is running.
bool Steam::isSteamRunning(void) {
	return SteamAPI_IsSteamRunning();
}
/////////////////////////////////////////////////
///// APPS //////////////////////////////////////

bool Steam::isSubscribed() {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsSubscribed();
}

bool Steam::isLowViolence() {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsLowViolence();
}

bool Steam::isCybercafe() {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsCybercafe();
}

bool Steam::isVACBanned() {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsVACBanned();
}

String Steam::getCurrentGameLanguage() {
	if (SteamApps() == NULL) {
		return "None";
	}
	return SteamApps()->GetCurrentGameLanguage();
}

String Steam::getAvailableGameLanguages() {
	if (SteamApps() == NULL) {
		return "None";
	}
	return SteamApps()->GetAvailableGameLanguages();
}

bool Steam::isSubscribedApp(int value) {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsSubscribedApp((AppId_t)value);
}

bool Steam::isDLCInstalled(int value) {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsDlcInstalled(value);
}

int Steam::getEarliestPurchaseUnixTime(int value) {
	if (SteamApps() == NULL) {
		return 0;
	}
	return SteamApps()->GetEarliestPurchaseUnixTime((AppId_t)value);
}

bool Steam::isSubscribedFromFreeWeekend() {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsSubscribedFromFreeWeekend();
}

int Steam::getDLCCount() {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->GetDLCCount();
}

void Steam::installDLC(int value) {
	if (SteamApps() == NULL) {
		return;
	}
	SteamApps()->InstallDLC((AppId_t)value);
}

void Steam::uninstallDLC(int value) {
	if (SteamApps() == NULL) {
		return;
	}
	SteamApps()->UninstallDLC((AppId_t)value);
}

String Steam::getCurrentBetaName() {
	String ret = "";
	if (SteamApps() != NULL) {
		char str[1024];
		if (SteamApps()->GetCurrentBetaName(str, 1024)) {
			ret = String(str);
		}
	}
	return ret;
}

bool Steam::markContentCorrupt(bool missingFilesOnly) {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->MarkContentCorrupt(missingFilesOnly);
}


String Steam::getAppInstallDir(AppId_t appID) {
	if (SteamApps() == NULL) {
		return "";
	}
	const uint32 folderBuffer = 256;
	char *buffer = new char[folderBuffer];
	SteamApps()->GetAppInstallDir(appID, (char *)buffer, folderBuffer);
	String appDir = buffer;
	delete buffer;
	return appDir;
}

bool Steam::isAppInstalled(int value) {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->BIsAppInstalled((AppId_t)value);
}

uint64_t Steam::getAppOwner() {
	if (SteamApps() == NULL) {
		return 0;
	}
	CSteamID cSteamID = SteamApps()->GetAppOwner();
	return cSteamID.ConvertToUint64();
}

String Steam::getLaunchQueryParam(const String &key) {
	if (SteamApps() == NULL) {
		return "";
	}
	return SteamApps()->GetLaunchQueryParam(key.utf8().get_data());
}

bool Steam::getDLCDownloadProgress(int appID, uint64 *bytesDownloaded, uint64 *bytesTotal) {
	if (SteamApps() == NULL) {
		return false;
	}
	return SteamApps()->GetDlcDownloadProgress((AppId_t)appID, bytesDownloaded, bytesTotal);
}

int Steam::getAppBuildId() {
	if (SteamApps() == NULL) {
		return 0;
	}
	return SteamApps()->GetAppBuildId();
}

void Steam::activateActionSet(uint64_t controllerHandle, uint64_t actionSetHandle) {
	if (SteamController() != NULL) {
		SteamController()->ActivateActionSet((ControllerHandle_t)controllerHandle, (ControllerActionSetHandle_t)actionSetHandle);
	}
}

uint64_t Steam::getActionSetHandle(const String &actionSetName) {
	if (SteamController() != NULL) {
		return (uint64_t)SteamController()->GetActionSetHandle(actionSetName.utf8().get_data());
	}
	return 0;
}

Dictionary Steam::getAnalogActionData(uint64_t controllerHandle, uint64_t analogActionHandle) {
	ControllerAnalogActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if (SteamController() != NULL) {
		data = SteamController()->GetAnalogActionData((ControllerHandle_t)controllerHandle, (ControllerAnalogActionHandle_t)analogActionHandle);
	}
	d["eMode"] = data.eMode;
	d["x"] = data.x;
	d["y"] = data.y;
	d["bActive"] = data.bActive;
	return d;
}

uint64_t Steam::getAnalogActionHandle(const String &actionName) {
	if (SteamController() != NULL) {
		return (uint64_t)SteamController()->GetAnalogActionHandle(actionName.utf8().get_data());
	}
	return 0;
}

Array Steam::getAnalogActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t analogActionHandle) {
	Array list;
	if (SteamController() != NULL) {
		EControllerActionOrigin out[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamController()->GetAnalogActionOrigins((ControllerHandle_t)controllerHandle, (ControllerActionSetHandle_t)actionSetHandle, (ControllerAnalogActionHandle_t)analogActionHandle, out);
		for (int i = 0; i < ret; i++) {
			list.push_back((int)out[i]);
		}
	}
	return list;
}

Array Steam::getConnectedControllers() {
	Array list;
	if (SteamController() != NULL) {
		ControllerHandle_t handles[STEAM_CONTROLLER_MAX_COUNT];
		int ret = SteamController()->GetConnectedControllers(handles);
		for (int i = 0; i < ret; i++) {
			list.push_back((uint64_t)handles[i]);
		}
	}
	return list;
}

uint64_t Steam::getControllerForGamepadIndex(int index) {
	if (SteamController() != NULL) {
		return (uint64_t)SteamController()->GetControllerForGamepadIndex(index);
	}
	return 0;
}

uint64_t Steam::getCurrentActionSet(uint64_t controllerHandle) {
	if (SteamController() != NULL) {
		return (uint64_t)SteamController()->GetCurrentActionSet((ControllerHandle_t)controllerHandle);
	}
	return 0;
}

Dictionary Steam::getDigitalActionData(uint64_t controllerHandle, uint64_t digitalActionHandle) {
	ControllerDigitalActionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if (SteamController() != NULL) {
		data = SteamController()->GetDigitalActionData((ControllerHandle_t)controllerHandle, (ControllerDigitalActionHandle_t)digitalActionHandle);
	}
	d["bState"] = data.bState;
	d["bActive"] = data.bActive;
	return d;
}

uint64_t Steam::getDigitalActionHandle(const String &actionName) {
	if (SteamController() != NULL) {
		return (uint64_t)SteamController()->GetDigitalActionHandle(actionName.utf8().get_data());
	}
	return 0;
}

Array Steam::getDigitalActionOrigins(uint64_t controllerHandle, uint64_t actionSetHandle, uint64_t digitalActionHandle) {
	Array list;
	if (SteamController() != NULL) {
		EControllerActionOrigin out[STEAM_CONTROLLER_MAX_ORIGINS];
		int ret = SteamController()->GetDigitalActionOrigins((ControllerHandle_t)controllerHandle, (ControllerActionSetHandle_t)actionSetHandle, (ControllerDigitalActionHandle_t)digitalActionHandle, out);
		for (int i = 0; i < ret; i++) {
			list.push_back((int)out[i]);
		}
	}
	return list;
}

int Steam::getGamepadIndexForController(uint64_t controllerHandle) {
	if (SteamController() != NULL) {
		return SteamController()->GetGamepadIndexForController((ControllerHandle_t)controllerHandle);
	}
	return -1;
}

Dictionary Steam::getMotionData(uint64_t controllerHandle) {
	ControllerMotionData_t data;
	Dictionary d;
	memset(&data, 0, sizeof(data));
	if (SteamController() != NULL) {
		data = SteamController()->GetMotionData((ControllerHandle_t)controllerHandle);
	}
	d["rotQuatX"] = data.rotQuatX;
	d["rotQuatY"] = data.rotQuatY;
	d["rotQuatZ"] = data.rotQuatZ;
	d["rotQuatW"] = data.rotQuatW;
	d["posAccelX"] = data.posAccelX;
	d["posAccelY"] = data.posAccelY;
	d["posAccelZ"] = data.posAccelZ;
	d["rotVelX"] = data.rotVelX;
	d["rotVelY"] = data.rotVelY;
	d["rotVelZ"] = data.rotVelZ;
	return d;
}

bool Steam::init() {
	if (SteamController() != NULL) {
		return SteamController()->Init();
	}
	return false;
}

void Steam::runFrame() {
	if (SteamController() != NULL) {
		SteamController()->RunFrame();
	}
}

bool Steam::showBindingPanel(uint64_t controllerHandle) {
	if (SteamController() != NULL) {
		return SteamController()->ShowBindingPanel((ControllerHandle_t)controllerHandle);
	}
	return false;
}

bool Steam::shutdown() {
	if (SteamController() != NULL) {
		return SteamController()->Shutdown();
	}
	return false;
}

void Steam::triggerVibration(uint64_t controllerHandle, uint16_t leftSpeed, uint16_t rightSpeed) {
	if (SteamController() != NULL) {
		SteamController()->TriggerVibration((ControllerHandle_t)controllerHandle, (unsigned short)leftSpeed, (unsigned short)rightSpeed);
	}
}

int Steam::getFriendCount() {
	if (SteamFriends() == NULL) {
		return 0;
	}
	return SteamFriends()->GetFriendCount(0x04);
}

String Steam::getPersonaName() {
	if (SteamFriends() == NULL) {
		return "";
	}
	return SteamFriends()->GetPersonaName();
}

String Steam::getFriendPersonaName(int steamID) {
	if (SteamFriends() != NULL && steamID > 0) {
		CSteamID friendID = createSteamID(steamID);
		bool isDataLoading = SteamFriends()->RequestUserInformation(friendID, true);
		if (!isDataLoading) {
			return SteamFriends()->GetFriendPersonaName(friendID);
		}
	}
	return "";
}

void Steam::setGameInfo(const String &key, const String &value) {
	if (SteamFriends() == NULL) {
		return;
	}
	SteamFriends()->SetRichPresence(key.utf8().get_data(), value.utf8().get_data());
}

void Steam::clearGameInfo() {
	if (SteamFriends() == NULL) {
		return;
	}
	SteamFriends()->ClearRichPresence();
}

void Steam::inviteFriend(int steamID, const String &connectString) {
	if (SteamFriends() == NULL) {
		return;
	}
	CSteamID friendID = createSteamID(steamID);

	SteamFriends()->InviteUserToGame(friendID, connectString.utf8().get_data());
}

void Steam::setPlayedWith(int steamID) {
	if (SteamFriends() == NULL) {
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->SetPlayedWith(friendID);
}

Array Steam::getRecentPlayers() {
	if (SteamFriends() == NULL) {
		return Array();
	}
	int count = SteamFriends()->GetCoplayFriendCount();
	Array recents;
	for (int i = 0; i < count; i++) {
		CSteamID playerID = SteamFriends()->GetCoplayFriend(i);
		if (SteamFriends()->GetFriendCoplayGame(playerID) == SteamUtils()->GetAppID()) {
			Dictionary player;
			String name = SteamFriends()->GetFriendPersonaName(playerID);
			int status = SteamFriends()->GetFriendPersonaState(playerID);
			player["id"] = playerID.GetAccountID();
			player["name"] = name;
			player["status"] = status;
			recents.append(player);
		}
	}
	return recents;
}

void Steam::getFriendAvatar(int size, int steamID) {
	if (size < AVATAR_SMALL || size > AVATAR_LARGE) {
		return;
	}
	if (SteamFriends() == NULL) {
		return;
	}

	int handle = -2;

	// Use the user ID if steamID is invalid
	CSteamID fetchID = (steamID < 0 ? SteamUser()->GetSteamID() : createSteamID(steamID));

	switch (size) {
		case AVATAR_SMALL: {
			handle = SteamFriends()->GetSmallFriendAvatar(fetchID);
			size = 32;
			break;
		}
		case AVATAR_MEDIUM: {
			handle = SteamFriends()->GetMediumFriendAvatar(fetchID);
			size = 64;
			break;
		}
		case AVATAR_LARGE: {
			handle = SteamFriends()->GetLargeFriendAvatar(fetchID);
			size = 184;
			break;
		}
		default:
			return;
	}

	// TODO: Is this block needed? Not so sure.
	// Handle is invalid
	if (handle <= 0) {
		return;
	}

	// Has already loaded, simulate callback.
	AvatarImageLoaded_t *avatarData = new AvatarImageLoaded_t;
	avatarData->m_steamID = fetchID;
	avatarData->m_iImage = handle;
	avatarData->m_iWide = size;
	avatarData->m_iTall = size;
	_avatar_loaded(avatarData);
	delete avatarData;
}

void Steam::_avatar_loaded(AvatarImageLoaded_t *avatarData) {

	// TODO: Pull this to the constructor? Lazy loaded rn
	// Ensure the images exist
	if (AvatarImage.is_null())
		AvatarImage.instance();

	if (AvatarImageTexture.is_null())
		AvatarImageTexture.instance();

	int size = avatarData->m_iWide;

	// Create image buffer
	int bufferSize = 4 * size * size;
	uint8 *buffer = new uint8[bufferSize];

	// Pull out the staged image
	bool success = SteamUtils()->GetImageRGBA(avatarData->m_iImage, buffer, bufferSize);

	if (!success) {
		printf("[Steam] Failed to load image buffer from callback\n");
		return;
	}

	int avatarSize;

	if (size == 32)
		avatarSize = AVATAR_SMALL;
	else if (size == 64)
		avatarSize = AVATAR_MEDIUM;
	else if (size == 184)
		avatarSize = AVATAR_LARGE;
	else {
		printf("[Steam] Invalid avatar size from callback\n");
		return;
	}

	PoolByteArray temppool;
	for (int i = 0; i < bufferSize; i++)
		temppool.append(buffer[i]);

	// Create the image, then image texture from that
	AvatarImage->create(size, size, false, Image::FORMAT_RGBA8, temppool);
	AvatarImageTexture.instance();
	AvatarImageTexture->create_from_image(AvatarImage);

	call_deferred("emit_signal", "avatar_loaded", avatarSize, AvatarImageTexture);
}

void Steam::activateGameOverlay(const String &url) {
	if (SteamFriends() == NULL) {
		return;
	}
	SteamFriends()->ActivateGameOverlay(url.utf8().get_data());
}

void Steam::activateGameOverlayToUser(const String &url, int steamID) {
	if (SteamFriends() == NULL) {
		return;
	}
	CSteamID overlayUserID = createSteamID(steamID);
	SteamFriends()->ActivateGameOverlayToUser(url.utf8().get_data(), overlayUserID);
}

void Steam::activateGameOverlayToWebPage(const String &url) {
	if (SteamFriends() == NULL) {
		return;
	}
	SteamFriends()->ActivateGameOverlayToWebPage(url.utf8().get_data());
}

void Steam::activateGameOverlayToStore(int appID) {
	if (SteamFriends() == NULL) {
		return;
	}
	SteamFriends()->ActivateGameOverlayToStore(AppId_t(appID), EOverlayToStoreFlag(0));
}

Array Steam::getUserSteamGroups() {
	if (SteamFriends() == NULL) {
		return Array();
	}
	int groupCount = SteamFriends()->GetClanCount();
	Array steamGroups;
	for (int i = 0; i < groupCount; i++) {
		Dictionary groups;
		CSteamID groupID = SteamFriends()->GetClanByIndex(i);
		String name = SteamFriends()->GetClanName(groupID);
		String tag = SteamFriends()->GetClanTag(groupID);
		groups["id"] = groupID.GetAccountID();
		groups["name"] = name;
		groups["tag"] = tag;
		steamGroups.append(groups);
	}
	return steamGroups;
}

Array Steam::getUserSteamFriends() {
	if (SteamFriends() == NULL) {
		return Array();
	}
	int count = SteamFriends()->GetFriendCount(0x04);
	Array steamFriends;
	for (int i = 0; i < count; i++) {
		Dictionary friends;
		CSteamID friendID = SteamFriends()->GetFriendByIndex(i, 0x04);
		String name = SteamFriends()->GetFriendPersonaName(friendID);
		int status = SteamFriends()->GetFriendPersonaState(friendID);
		friends["id"] = friendID.GetAccountID();
		friends["name"] = name;
		friends["status"] = status;
		steamFriends.append(friends);
	}
	return steamFriends;
}
void Steam::activateGameOverlayInviteDialog(int steamID) {
	if (SteamFriends() == NULL) {
		return;
	}
	CSteamID lobbyID = createSteamID(steamID);
	SteamFriends()->ActivateGameOverlayInviteDialog(lobbyID);
}

void Steam::createLobby(int lobbyType, int maxMembers) {
	if (SteamMatchmaking() == NULL) {
		return;
	}
	ELobbyType eLobbyType;

	// Convert the lobby type back over.
	// TODO: Is there a more elegant way to convert these enums?
	if (lobbyType == LOBBY_TYPE_PRIVATE)
		eLobbyType = k_ELobbyTypePrivate;
	else if (lobbyType == LOBBY_TYPE_FRIENDS_ONLY)
		eLobbyType = k_ELobbyTypeFriendsOnly;
	else if (lobbyType == LOBBY_TYPE_PUBLIC)
		eLobbyType = k_ELobbyTypePublic;
	else
		eLobbyType = k_ELobbyTypeInvisible;

	SteamMatchmaking()->CreateLobby(eLobbyType, maxMembers);
}

void Steam::joinLobby(int steamIDLobby) {
	if (SteamMatchmaking() == NULL) {
		return;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	SteamMatchmaking()->JoinLobby(lobbyID);
}

void Steam::leaveLobby(int steamIDLobby) {
	if (SteamMatchmaking() == NULL) {
		return;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	return SteamMatchmaking()->LeaveLobby(lobbyID);
}

bool Steam::inviteUserToLobby(int steamIDLobby, int steamIDInvitee) {
	if (SteamMatchmaking() == NULL) {
		return 0;
	}
	CSteamID lobbyID = createSteamID(steamIDLobby);
	CSteamID inviteeID = createSteamID(steamIDInvitee);
	return SteamMatchmaking()->InviteUserToLobby(lobbyID, inviteeID);
}

bool Steam::musicIsEnabled() {
	if (SteamMusic() == NULL) {
		return false;
	}
	return SteamMusic()->BIsEnabled();
}

bool Steam::musicIsPlaying() {
	if (SteamMusic() == NULL) {
		return false;
	}
	return SteamMusic()->BIsPlaying();
}

float Steam::musicGetVolume() {
	if (SteamMusic() == NULL) {
		return 0;
	}
	return SteamMusic()->GetVolume();
}

void Steam::musicPause() {
	if (SteamMusic() == NULL) {
		return;
	}
	return SteamMusic()->Pause();
}

void Steam::musicPlay() {
	if (SteamMusic() == NULL) {
		return;
	}
	return SteamMusic()->Play();
}

void Steam::musicPlayNext() {
	if (SteamMusic() == NULL) {
		return;
	}
	return SteamMusic()->PlayNext();
}

void Steam::musicPlayPrev() {
	if (SteamMusic() == NULL) {
		return;
	}
	return SteamMusic()->PlayPrevious();
}

void Steam::musicSetVolume(float value) {
	if (SteamMusic() == NULL) {
		return;
	}
	return SteamMusic()->SetVolume(value);
}


bool Steam::fileWrite(const String &file, const PoolByteArray &data, int32_t dataSize) {
	if (SteamRemoteStorage() == NULL) {
		return false;
	}
	return SteamRemoteStorage()->FileWrite(file.utf8().get_data(), data.read().ptr(), dataSize);
}

Dictionary Steam::fileRead(const String &file, int32_t dataToRead) {
	Dictionary d;
	if (SteamRemoteStorage() == NULL) {
		d["ret"] = false;
		return d;
	}
	PoolByteArray data;
	data.resize(dataToRead);
	d["ret"] = SteamRemoteStorage()->FileRead(file.utf8().get_data(), data.write().ptr(), dataToRead);
	d["buf"] = data;
	return d;
}

bool Steam::fileForget(const String &file) {
	if (SteamRemoteStorage() == NULL) {
		return false;
	}
	return SteamRemoteStorage()->FileForget(file.utf8().get_data());
}

bool Steam::fileDelete(const String &file) {
	if (SteamRemoteStorage() == NULL) {
		return false;
	}
	return SteamRemoteStorage()->FileDelete(file.utf8().get_data());
}

bool Steam::fileExists(const String &file) {
	if (SteamRemoteStorage() == NULL) {
		return false;
	}
	return SteamRemoteStorage()->FileExists(file.utf8().get_data());
}

bool Steam::filePersisted(const String &file) {
	if (SteamRemoteStorage() == NULL) {
		return false;
	}
	return SteamRemoteStorage()->FilePersisted(file.utf8().get_data());
}

int32_t Steam::getFileSize(const String &file) {
	if (SteamRemoteStorage() == NULL) {
		return -1;
	}
	return SteamRemoteStorage()->GetFileSize(file.utf8().get_data());
}

int64_t Steam::getFileTimestamp(const String &file) {
	if (SteamRemoteStorage() == NULL) {
		return -1;
	}
	return SteamRemoteStorage()->GetFileTimestamp(file.utf8().get_data());
}

int32_t Steam::getFileCount() {
	if (SteamRemoteStorage() == NULL) {
		return 0;
	}
	return SteamRemoteStorage()->GetFileCount();
}

Dictionary Steam::getFileNameAndSize(int file) {
	Dictionary d;
	String name = "";
	int32_t size = 0;
	if (SteamRemoteStorage() != NULL) {
		name = String(SteamRemoteStorage()->GetFileNameAndSize(file, &size));
	}
	d["name"] = name;
	d["size"] = size;
	return d;
}

Dictionary Steam::getQuota() {
	Dictionary d;
	uint64_t total = 0;
	uint64_t available = 0;
	if (SteamRemoteStorage() != NULL) {
		SteamRemoteStorage()->GetQuota((uint64 *)&total, (uint64 *)&available);
	}
	d["total_bytes"] = total;
	d["available_bytes"] = available;
	return d;
}

uint32_t Steam::getSyncPlatforms(const String &file) {
	if (SteamRemoteStorage() == NULL) {
		return 0;
	}
	return SteamRemoteStorage()->GetSyncPlatforms(file.utf8().get_data());
}

bool Steam::isCloudEnabledForAccount() {
	if (SteamRemoteStorage() == NULL) {
		return false;
	}
	return SteamRemoteStorage()->IsCloudEnabledForAccount();
}

bool Steam::isCloudEnabledForApp() {
	if (SteamRemoteStorage() == NULL) {
		return false;
	}
	return SteamRemoteStorage()->IsCloudEnabledForApp();
}

void Steam::setCloudEnabledForApp(bool enabled) {
	if (SteamRemoteStorage() == NULL) {
		return;
	}
	return SteamRemoteStorage()->SetCloudEnabledForApp(enabled);
}


uint32_t Steam::addScreenshotToLibrary(const String &filename, const String &thumbnailFilename, int width, int height) {
	if (SteamScreenshots() == NULL) {
		return 0;
	}
	return SteamScreenshots()->AddScreenshotToLibrary(filename.utf8().get_data(), thumbnailFilename.utf8().get_data(), width, height);
}

void Steam::hookScreenshots(bool hook) {
	if (SteamScreenshots() == NULL) {
		return;
	}
	SteamScreenshots()->HookScreenshots(hook);
}

bool Steam::isScreenshotsHooked() {
	if (SteamScreenshots() == NULL) {
		return false;
	}
	return SteamScreenshots()->IsScreenshotsHooked();
}

bool Steam::setLocation(uint32_t screenshot, const String &location) {
	if (SteamScreenshots() == NULL) {
		return false;
	}
	ScreenshotHandle handle = (ScreenshotHandle)screenshot;
	return SteamScreenshots()->SetLocation(handle, location.utf8().get_data());
}

void Steam::triggerScreenshot() {
	if (SteamScreenshots() == NULL) {
		return;
	}
	SteamScreenshots()->TriggerScreenshot();
}

uint32_t Steam::writeScreenshot(const PoolByteArray &RGB, int width, int height) {
	if (SteamScreenshots() == NULL) {
		return 0;
	}
	return SteamScreenshots()->WriteScreenshot((void *)RGB.read().ptr(), RGB.size(), width, height);
}

uint32_t Steam::getAuthSessionTicket() {
	if (SteamUser() == NULL) {
		return 0;
	}
	uint32_t ticketSize = 1024;
	uint32_t *buffer = memnew_arr(uint32_t, ticketSize);
	uint32_t id = SteamUser()->GetAuthSessionTicket(buffer, ticketSize, &ticketSize);
	TicketData ticket = { id, buffer, ticketSize };
	tickets.push_back(ticket);
	return id;
}

void Steam::cancelAuthTicket(uint32_t authTicket) {
	if (SteamUser() == NULL) {
		return;
	}
	for (int i = 0; i < tickets.size(); i++) {
		TicketData ticket = tickets.get(i);
		if (ticket.id == authTicket) {
			tickets.remove(i);
			memdelete_arr(ticket.buffer);
			break;
		}
	}
}

int Steam::beginAuthSession(uint32_t authTicket, uint64_t steamID) {
	if (SteamUser() == NULL) {
		return -1;
	}
	for (int i = 0; i < tickets.size(); i++) {
		TicketData ticket = tickets.get(i);
		if (ticket.id == authTicket) {
			CSteamID authSteamID = createSteamID(steamID);
			return SteamUser()->BeginAuthSession(ticket.buffer, ticket.size, authSteamID);
		}
	}
	return -1;
}

void Steam::endAuthSession(uint64_t steamID) {
	if (SteamUser() == NULL) {
		return;
	}
	CSteamID authSteamID = createSteamID(steamID);
	return SteamUser()->EndAuthSession(authSteamID);
}

uint64_t Steam::getSteamID() {
	if (SteamUser() == NULL) {
		return 0;
	}
	CSteamID cSteamID = SteamUser()->GetSteamID();
	return cSteamID.ConvertToUint64();
}

bool Steam::loggedOn() {
	if (SteamUser() == NULL) {
		return false;
	}
	return SteamUser()->BLoggedOn();
}

int Steam::getPlayerSteamLevel() {
	if (SteamUser() == NULL) {
		return 0;
	}
	return SteamUser()->GetPlayerSteamLevel();
}

String Steam::getUserDataFolder() {
	if (SteamUser() == NULL) {
		return "";
	}
	const int bufferSize = 256;
	char *buffer = new char[bufferSize];
	SteamUser()->GetUserDataFolder((char *)buffer, bufferSize);
	String data_path = buffer;
	delete buffer;
	return data_path;
}

void Steam::advertiseGame(const String &serverIP, int port) {
	if (SteamUser() == NULL) {
		return;
	}
	// Resolve address and convert it from IP_Address struct to uint32_t
	IP_Address address;
	if (serverIP.is_valid_ip_address()) {
		address = serverIP;
	} else {
		address = IP::get_singleton()->resolve_hostname(serverIP, IP::TYPE_IPV4);
	}
	// Resolution failed - Godot 3.0 has is_invalid() to check this
	if (address == IP_Address()) {
		return;
	}
	uint32_t ip4 = *((uint32_t *)address.get_ipv4());
	// Swap the bytes
	uint8_t *ip4_p = (uint8_t *)&ip4;
	for (int i = 0; i < 2; i++) {
		uint8_t temp = ip4_p[i];
		ip4_p[i] = ip4_p[3 - i];
		ip4_p[3 - i] = temp;
	}
	CSteamID gameserverID = SteamUser()->GetSteamID();
	SteamUser()->AdvertiseGame(gameserverID, *((uint32_t *)ip4_p), port);
}

int Steam::getGameBadgeLevel(int series, bool foil) {
	if (SteamUser() == NULL) {
		return 0;
	}
	return SteamUser()->GetGameBadgeLevel(series, foil);
}


bool Steam::clearAchievement(const String &name) {
	if (SteamUserStats() == NULL) {
		return false;
	}
	return SteamUserStats()->ClearAchievement(name.utf8().get_data());
}

bool Steam::getAchievement(const String &name) {
	bool achieved = false;
	return SteamUserStats()->GetAchievement(name.utf8().get_data(), &achieved);
}

Dictionary Steam::getAchievementAchievedPercent(const String &name) {
	Dictionary d;
	float percent = 0.f;
	bool achieved = false;
	if (SteamUserStats() == NULL) {
		d["ret"] = false;
	} else {
		d["ret"] = SteamUserStats()->GetAchievementAchievedPercent(name.utf8().get_data(), &percent);
	}
	d["percent"] = percent;
	return d;
}

String Steam::getAchievementDisplayAttribute(const String &name, const String &key) {
	if (SteamUserStats() == NULL) {
		return "";
	}
	return SteamUserStats()->GetAchievementDisplayAttribute(name.utf8().get_data(), key.utf8().get_data());
}

int Steam::getAchievementIcon(const String &name) {
	if (SteamUserStats() == NULL) {
		return 0;
	}
	return SteamUserStats()->GetAchievementIcon(name.utf8().get_data());
}

String Steam::getAchievementName(uint32_t iAchievement) {
	if (SteamUserStats() == NULL) {
		return "";
	}
	return SteamUserStats()->GetAchievementName((uint32)iAchievement);
}

void Steam::getNumberOfCurrentPlayers() {
	if (SteamUserStats() == NULL) {
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->GetNumberOfCurrentPlayers();
	callResultNumberOfCurrentPlayers.Set(apiCall, this, &Steam::_number_of_current_players);
}

uint32_t Steam::getNumAchievements() {
	if (SteamUserStats() == NULL) {
		return 0;
	}
	return SteamUserStats()->GetNumAchievements();
}

float Steam::getStatFloat(const String &name) {
	float statValue = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statValue);
	return statValue;
}

int Steam::getStatInt(const String &name) {
	int32_t statValue = 0;
	SteamUserStats()->GetStat(name.utf8().get_data(), &statValue);
	return statValue;
}

bool Steam::resetAllStats(bool achievementsToo) {
	SteamUserStats()->ResetAllStats(achievementsToo);
	return SteamUserStats()->StoreStats();
}

bool Steam::requestCurrentStats() {
	return SteamUserStats()->RequestCurrentStats();
}

void Steam::requestGlobalAchievementPercentages() {
	if (SteamUserStats() == NULL) {
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->RequestGlobalAchievementPercentages();
	callResultGlobalAchievementPercentagesReady.Set(apiCall, this, &Steam::_global_achievement_percentages_ready);
}

bool Steam::setAchievement(const String &name) {
	if (SteamUserStats() == NULL) {
		return 0;
	}
	SteamUserStats()->SetAchievement(name.utf8().get_data());
	return SteamUserStats()->StoreStats();
}

bool Steam::setStatFloat(const String &name, float value) {
	return SteamUserStats()->SetStat(name.utf8().get_data(), value);
}

bool Steam::setStatInt(const String &name, int value) {
	return SteamUserStats()->SetStat(name.utf8().get_data(), value);
}

bool Steam::storeStats() {
	if (SteamUserStats() == NULL) {
		return 0;
	}
	SteamUserStats()->StoreStats();
	return SteamUserStats()->RequestCurrentStats();
}

void Steam::findLeaderboard(const String &name) {
	if (SteamUserStats() == NULL) {
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->FindLeaderboard(name.utf8().get_data());
	callResultFindLeaderboard.Set(apiCall, this, &Steam::_leaderboard_loaded);
}

String Steam::getLeaderboardName() {
	if (SteamUserStats() == NULL) {
		return "";
	}
	return SteamUserStats()->GetLeaderboardName(leaderboard_handle);
}

int Steam::getLeaderboardEntryCount() {
	if (SteamUserStats() == NULL) {
		return -1;
	}
	return SteamUserStats()->GetLeaderboardEntryCount(leaderboard_handle);
}

void Steam::downloadLeaderboardEntries(int start, int end, int type) {
	if (SteamUserStats() == NULL) {
		return;
	}
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntries(leaderboard_handle, ELeaderboardDataRequest(type), start, end);
	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
}

void Steam::downloadLeaderboardEntriesForUsers(Array usersID) {
	if (SteamUserStats() == NULL) {
		return;
	}
	int usersCount = usersID.size();
	if (usersCount == 0) {
		return;
	}
	CSteamID *users = new CSteamID[usersCount];
	for (int i = 0; i < usersCount; i++) {
		CSteamID user = createSteamID(usersID[i]);
		users[i] = user;
	}
	SteamAPICall_t apiCall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboard_handle, users, usersCount);
	callResultEntries.Set(apiCall, this, &Steam::_leaderboard_entries_loaded);
	delete[] users;
}

void Steam::uploadLeaderboardScore(int score, bool keepBest) {
	if (SteamUserStats() == NULL) {
		return;
	}
	ELeaderboardUploadScoreMethod method = keepBest ? k_ELeaderboardUploadScoreMethodKeepBest : k_ELeaderboardUploadScoreMethodForceUpdate;
	SteamAPICall_t apiCall = SteamUserStats()->UploadLeaderboardScore(leaderboard_handle, method, (int32)score, NULL, 0);
	callResultUploadScore.Set(apiCall, this, &Steam::_leaderboard_uploaded);
}

void Steam::getDownloadedLeaderboardEntry(SteamLeaderboardEntries_t handle, int entryCount) {
	if (SteamUserStats() == NULL) {
		return;
	}
	leaderboard_entries.clear();
	LeaderboardEntry_t *entry = memnew(LeaderboardEntry_t);
	for (int i = 0; i < entryCount; i++) {
		SteamUserStats()->GetDownloadedLeaderboardEntry(handle, i, entry, NULL, 0);
		Dictionary entryDict;
		entryDict["score"] = entry->m_nScore;
		entryDict["steamID"] = entry->m_steamIDUser.GetAccountID();
		entryDict["global_rank"] = entry->m_nGlobalRank;
		leaderboard_entries.append(entryDict);
	}
	memdelete(entry);
}

uint64_t Steam::getLeaderboardHandle() {
	return leaderboard_handle;
}

Array Steam::getLeaderboardEntries() {
	return leaderboard_entries;
}

bool Steam::getAchievementAndUnlockTime(const String &name, bool achieved, uint32_t unlockTime) {
	if (SteamUserStats() == NULL) {
		return 0;
	}
	return SteamUserStats()->GetAchievementAndUnlockTime(name.utf8().get_data(), (bool *)achieved, (uint32_t *)unlockTime);
}

bool Steam::indicateAchievementProgress(const String &name, int currentProgress, int maxProgress) {
	if (SteamUserStats() == NULL) {
		return 0;
	}
	return SteamUserStats()->IndicateAchievementProgress(name.utf8().get_data(), currentProgress, maxProgress);
}


String Steam::getIPCountry() {
	return SteamUtils()->GetIPCountry();
}

bool Steam::isOverlayEnabled() {
	return SteamUtils()->IsOverlayEnabled();
}

void Steam::setOverlayNotificationPosition(int pos) {
	if ((pos < 0) || (pos > 3) || (SteamUtils() == NULL)) {
		return;
	}
	SteamUtils()->SetOverlayNotificationPosition(ENotificationPosition(pos));
}

String Steam::getSteamUILanguage() {
	return SteamUtils()->GetSteamUILanguage();
}

int Steam::getAppID() {
	if (SteamUtils() == NULL) {
		return 0;
	}
	return SteamUtils()->GetAppID();
}

Dictionary Steam::getImageRGBA(int image) {
	Dictionary d;
	bool ret = false;
	if (SteamUtils() != NULL) {
		uint32 width;
		uint32 height;
		ret = SteamUtils()->GetImageSize(image, &width, &height);
		if (ret) {
			PoolByteArray data;
			data.resize(width * height * 4);
			ret = SteamUtils()->GetImageRGBA(image, data.write().ptr(), data.size());
			if (ret) {
				d["buf"] = data;
			}
		}
	}
	d["ret"] = ret;
	return d;
}

Dictionary Steam::getImageSize(int image) {
	Dictionary d;
	bool ret = false;
	if (SteamUtils() != NULL) {
		uint32 width;
		uint32 height;
		ret = SteamUtils()->GetImageSize(image, &width, &height);
		d["width"] = width;
		d["height"] = height;
	}
	d["ret"] = ret;
	return d;
}

int Steam::getSecondsSinceAppActive() {
	if (SteamUtils() == NULL) {
		return 0;
	}
	return SteamUtils()->GetSecondsSinceAppActive();
}

int Steam::getCurrentBatteryPower() {
	if (SteamUtils() == NULL) {
		return 0;
	}
	return SteamUtils()->GetCurrentBatteryPower();
}

bool Steam::isSteamRunningInVR() {
	if (SteamUtils() == NULL) {
		return 0;
	}
	return SteamUtils()->IsSteamRunningInVR();
}

int Steam::getServerRealTime() {
	if (SteamUtils() == NULL) {
		return 0;
	}
	return SteamUtils()->GetServerRealTime();
}

bool Steam::isSteamInBigPictureMode() {
	if (SteamUtils() == NULL) {
		return false;
	}
	return SteamUtils()->IsSteamInBigPictureMode();
}

void Steam::startVRDashboard() {
	if (SteamUtils() == NULL) {
		return;
	}
	SteamUtils()->StartVRDashboard();
}

bool Steam::downloadItem(int publishedFileID, bool highPriority) {
	if (SteamUGC() == NULL) {
		return 0;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->DownloadItem(fileID, highPriority);
}
void Steam::suspendDownloads(bool suspend) {
	return SteamUGC()->SuspendDownloads(suspend);
}

uint64_t Steam::startItemUpdate(AppId_t appID, int publishedFileID) {
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->StartItemUpdate(appID, fileID);
}

int Steam::getItemState(int publishedFileID) {
	if (SteamUGC() == NULL) {
		return 0;
	}
	PublishedFileId_t fileID = (int)publishedFileID;
	return SteamUGC()->GetItemState(fileID);
}

void Steam::createItem(AppId_t appID, int fileType) {
	if (SteamUGC() == NULL) {
		return;
	}
	EWorkshopFileType workshopType;
	// Convert the file type back over.
	if (fileType == UGC_ITEM_MAX) {
		workshopType = k_EWorkshopFileTypeMax;
	} else if (fileType == UGC_ITEM_MICROTRANSACTION) {
		workshopType = k_EWorkshopFileTypeMicrotransaction;
	} else if (fileType == UGC_ITEM_COLLECTION) {
		workshopType = k_EWorkshopFileTypeCollection;
	} else if (fileType == UGC_ITEM_ART) {
		workshopType = k_EWorkshopFileTypeArt;
	} else if (fileType == UGC_ITEM_VIDEO) {
		workshopType = k_EWorkshopFileTypeVideo;
	} else if (fileType == UGC_ITEM_SCREENSHOT) {
		workshopType = k_EWorkshopFileTypeScreenshot;
	} else if (fileType == UGC_ITEM_GAME) {
		workshopType = k_EWorkshopFileTypeGame;
	} else if (fileType == UGC_ITEM_SOFTWARE) {
		workshopType = k_EWorkshopFileTypeSoftware;
	} else if (fileType == UGC_ITEM_CONCEPT) {
		workshopType = k_EWorkshopFileTypeConcept;
	} else if (fileType == UGC_ITEM_WEBGUIDE) {
		workshopType = k_EWorkshopFileTypeWebGuide;
	} else if (fileType == UGC_ITEM_INTEGRATEDGUIDE) {
		workshopType = k_EWorkshopFileTypeIntegratedGuide;
	} else if (fileType == UGC_ITEM_MERCH) {
		workshopType = k_EWorkshopFileTypeMerch;
	} else if (fileType == UGC_ITEM_CONTROLLERBINDING) {
		workshopType = k_EWorkshopFileTypeControllerBinding;
	} else if (fileType == UGC_ITEM_STEAMWORKSACCESSINVITE) {
		workshopType = k_EWorkshopFileTypeSteamworksAccessInvite;
	} else if (fileType == UGC_ITEM_STEAMVIDEO) {
		workshopType = k_EWorkshopFileTypeSteamVideo;
	} else if (fileType == UGC_ITEM_GAMEMANAGEDITEM) {
		workshopType = k_EWorkshopFileTypeGameManagedItem;
	} else {
		workshopType = k_EWorkshopFileTypeCommunity;
	}
	SteamAPICall_t apiCall = SteamUGC()->CreateItem(appID, workshopType);
	callResultItemCreate.Set(apiCall, this, &Steam::_workshop_item_created);
}

bool Steam::setItemTitle(uint64_t updateHandle, const String &title) {
	if (SteamUGC() == NULL) {
		return false;
	}
	if (title.length() > UGC_MAX_TITLE_CHARS) {
		printf("Title cannot have more than %d ASCII characters. Title not set.", UGC_MAX_TITLE_CHARS);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemTitle(handle, title.utf8().get_data());
}

bool Steam::setItemDescription(uint64_t updateHandle, const String &description) {
	if (SteamUGC() == NULL) {
		return false;
	}
	if (description.length() > UGC_MAX_DESC_CHARS) {
		printf("Description cannot have more than %d ASCII characters. Description not set.", UGC_MAX_DESC_CHARS);
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemDescription(handle, description.utf8().get_data());
}

bool Steam::setItemUpdateLanguage(uint64_t updateHandle, const String &language) {
	if (SteamUGC() == NULL) {
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemUpdateLanguage(handle, language.utf8().get_data());
}
bool Steam::setItemMetadata(uint64_t updateHandle, const String &metadata) {
	if (SteamUGC() == NULL) {
		return false;
	}
	if (metadata.length() > UGC_MAX_METADATA_CHARS) {
		printf("Metadata cannot have more than %d ASCII characters. Metadata not set.", UGC_MAX_METADATA_CHARS);
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemMetadata(handle, metadata.utf8().get_data());
}

bool Steam::setItemVisibility(uint64_t updateHandle, int visibility) {
	if (SteamUGC() == NULL) {
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	ERemoteStoragePublishedFileVisibility itemVisibility;
	// Convert the visibility type back over.
	if (visibility == UGC_FILE_VISIBLE_PUBLIC) {
		itemVisibility = k_ERemoteStoragePublishedFileVisibilityPublic;
	} else if (visibility == UGC_FILE_VISIBLE_FRIENDS) {
		itemVisibility = k_ERemoteStoragePublishedFileVisibilityFriendsOnly;
	} else {
		itemVisibility = k_ERemoteStoragePublishedFileVisibilityPrivate;
	}
	return SteamUGC()->SetItemVisibility(handle, itemVisibility);
}


bool Steam::setItemContent(uint64_t updateHandle, const String &contentFolder) {
	if (SteamUGC() == NULL) {
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemContent(handle, contentFolder.utf8().get_data());
}

bool Steam::setItemPreview(uint64_t updateHandle, const String &previewFile) {
	if (SteamUGC() == NULL) {
		return false;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	return SteamUGC()->SetItemPreview(handle, previewFile.utf8().get_data());
}
void Steam::submitItemUpdate(uint64_t updateHandle, const String &changeNote) {
	if (SteamUGC() == NULL) {
		return;
	}
	UGCUpdateHandle_t handle = uint64(updateHandle);
	SteamAPICall_t apiCall = SteamUGC()->SubmitItemUpdate(handle, changeNote.utf8().get_data());
	callResultItemUpdate.Set(apiCall, this, &Steam::_workshop_item_updated);
}

void Steam::_file_details_result(FileDetailsResult_t *fileData) {
	uint32_t result = fileData->m_eResult;
	uint64_t fileSize = fileData->m_ulFileSize;
	int fileHash = fileData->m_FileSHA[20];
	uint32_t flags = fileData->m_unFlags;
	emit_signal("file_details_result", result, fileSize, fileHash, flags);
}

void Steam::_lobby_created(LobbyCreated_t *lobbyData) {
	int connect;
	// Convert the lobby response back over.
	if (lobbyData->m_eResult == k_EResultOK) {
		connect = LOBBY_OK;
	} else if (lobbyData->m_eResult == k_EResultNoConnection) {
		connect = LOBBY_NO_CONNECTION;
	} else if (lobbyData->m_eResult == k_EResultTimeout) {
		connect = LOBBY_TIMEOUT;
	} else if (lobbyData->m_eResult == k_EResultFail) {
		connect = LOBBY_FAIL;
	} else if (lobbyData->m_eResult == k_EResultAccessDenied) {
		connect = LOBBY_ACCESS_DENIED;
	} else {
		connect = LOBBY_LIMIT_EXCEEDED;
	}
	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
	emit_signal("lobby_created", connect, lobbyID);
}

void Steam::_lobby_joined(LobbyEnter_t *lobbyData) {
	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
	uint32_t permissions = lobbyData->m_rgfChatPermissions;
	bool locked = lobbyData->m_bLocked;
	uint32_t response = lobbyData->m_EChatRoomEnterResponse;
	emit_signal("lobby_joined", lobbyID, permissions, locked, response);
}

void Steam::_lobby_invite(LobbyInvite_t *lobbyData) {
	int inviterID = (uint64)lobbyData->m_ulSteamIDUser;
	int lobbyID = (uint64)lobbyData->m_ulSteamIDLobby;
	int gameID = (uint64)lobbyData->m_ulGameID;
	emit_signal("lobby_invite", inviterID, lobbyID, gameID);
}

void Steam::_join_requested(GameRichPresenceJoinRequested_t *callData) {
	int steamID = callData->m_steamIDFriend.GetAccountID();
	String con_string = callData->m_rgchConnect;
	emit_signal("join_requested", steamID, con_string);
}

// Signal number of current players (online + offline).
void Steam::_number_of_current_players(NumberOfCurrentPlayers_t *callData, bool bIOFailure) {
	emit_signal("number_of_current_players", callData->m_bSuccess && bIOFailure, callData->m_cPlayers);
}

void Steam::_leaderboard_loaded(LeaderboardFindResult_t *callData, bool bIOFailure) {
	leaderboard_handle = callData->m_hSteamLeaderboard;
	uint8_t found = callData->m_bLeaderboardFound;
	emit_signal("leaderboard_loaded", (uint64_t)leaderboard_handle, found);
}

void Steam::_leaderboard_uploaded(LeaderboardScoreUploaded_t *callData, bool bIOFailure) {
	// Incorrect leaderboard
	if (callData->m_hSteamLeaderboard != leaderboard_handle) {
		return;
	}
	emit_signal("leaderboard_uploaded", callData->m_bSuccess && bIOFailure, callData->m_nScore, callData->m_bScoreChanged, callData->m_nGlobalRankNew, callData->m_nGlobalRankPrevious);
}

void Steam::_leaderboard_entries_loaded(LeaderboardScoresDownloaded_t *callData, bool bIOFailure) {
	// Incorrect leaderboard
	if (callData->m_hSteamLeaderboard != leaderboard_handle) {
		return;
	}
	getDownloadedLeaderboardEntry(callData->m_hSteamLeaderboardEntries, callData->m_cEntryCount);
	emit_signal("leaderboard_entries_loaded");
}

void Steam::_overlay_toggled(GameOverlayActivated_t *callData) {
	if (callData->m_bActive) {
		emit_signal("overlay_toggled", true);
	} else {
		emit_signal("overlay_toggled", false);
	}
}

void Steam::_low_power(LowBatteryPower_t *timeLeft) {
	uint8 power = timeLeft->m_nMinutesBatteryLeft;
	emit_signal("low_power", power);
}

void Steam::_server_connected(SteamServersConnected_t *connectData) {
	emit_signal("connection_changed", true);
}

void Steam::_server_disconnected(SteamServersDisconnected_t *connectData) {
	emit_signal("connection_changed", false);
}

void Steam::_dlc_installed(DlcInstalled_t *callData) {
	int appID = (AppId_t)callData->m_nAppID;
	emit_signal("dlc_installed", appID);
}

void Steam::_get_auth_session_ticket_response(GetAuthSessionTicketResponse_t *callData) {
	emit_signal("get_auth_session_ticket_response", callData->m_hAuthTicket, callData->m_eResult);
}

void Steam::_validate_auth_ticket_response(ValidateAuthTicketResponse_t *callData) {
	uint64_t authID = callData->m_SteamID.ConvertToUint64();
	uint32_t response = callData->m_eAuthSessionResponse;
	uint64_t ownerID = callData->m_OwnerSteamID.ConvertToUint64();
	emit_signal("validate_auth_ticket_response", authID, response, ownerID);
}

void Steam::_screenshot_ready(ScreenshotReady_t *callData) {
	uint32_t handle = callData->m_hLocal;
	uint32_t result = callData->m_eResult;
	emit_signal("screenshot_ready", handle, result);
}

void Steam::_user_stats_received(UserStatsReceived_t *callData) {
	uint64_t gameID = callData->m_nGameID;
	uint32_t result = callData->m_eResult;
	uint64_t userID = callData->m_steamIDUser.ConvertToUint64();
	emit_signal("user_stats_received", gameID, result, userID);
}

void Steam::_user_achievement_icon_fetched(UserAchievementIconFetched_t *callData) {
	uint64_t gameID = callData->m_nGameID.ToUint64();
	String achievementName = callData->m_rgchAchievementName;
	bool achieved = callData->m_bAchieved;
	int iconHandle = callData->m_nIconHandle;
	emit_signal("user_achievement_icon_fetched", gameID, achievementName, achieved, iconHandle);
}

void Steam::_global_achievement_percentages_ready(GlobalAchievementPercentagesReady_t *callData, bool bIOFailure) {
	uint64_t gameID = callData->m_nGameID;
	uint32_t result = callData->m_eResult;
	emit_signal("global_achievement_percentages_ready", gameID, result);
}

void Steam::_workshop_item_created(CreateItemResult_t *callData, bool bIOFailure) {
	EResult result = callData->m_eResult;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("workshop_item_created", result, (uint64_t)fileID, acceptTOS);
}

void Steam::_workshop_item_updated(SubmitItemUpdateResult_t *callData, bool bIOFailure) {
	EResult result = callData->m_eResult;
	bool acceptTOS = callData->m_bUserNeedsToAcceptWorkshopLegalAgreement;
	emit_signal("workshop_item_updated", result, acceptTOS);
}

void Steam::_workshop_item_installed(ItemInstalled_t *callData) {
	AppId_t appID = callData->m_unAppID;
	PublishedFileId_t fileID = callData->m_nPublishedFileId;
	emit_signal("workshop_item_installed", appID, (uint64_t)fileID);
}

Steam::~Steam() {
	if (isInitSuccess) {
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	for (int i = 0; i < tickets.size(); i++) {
		TicketData ticket = tickets.get(i);
		memdelete_arr(ticket.buffer);
	}
	tickets.clear();
	singleton = NULL;
}
