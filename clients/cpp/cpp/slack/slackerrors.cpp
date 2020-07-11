#include "slackerrors.h"
#include "web/slack_response.h"

namespace slack {
	const std::string SlackApiError::what() const noexcept {
		const std::string server = "\nThe server responded with: \n";
		return SlackClientError::what() + server + server_response;
	};
};