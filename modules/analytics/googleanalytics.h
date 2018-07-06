#pragma once

#include "http_request.h"
#include "http_client.h"
#include "analytics.h"

//////////////////////////////////////////////////////////////////////////
// An implementation for google analytics
// Implemented custom using simple HTTP POST requests to preserve cross platform
// compatability. Most of google's implementations are restricted to mobile.
// https://developers.google.com/analytics/devguides/collection/protocol/v1/
//////////////////////////////////////////////////////////////////////////

class GoogleAnalytics : public AnalyticsSystem {

	void StartSession() {

	}

	void EndSession() {

	}

	void RecordScreen(String Name) {

	}

	void RecordEvent(String Name) {

	}

	void RecordEventWithAttribute(String EventName, String AttributeName, String AttributeValue) {

	}

	void FlushCache() {

	}
};
