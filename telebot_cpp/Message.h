#pragma once

#include "pch.h"

#include "Chat.h"

class Message
{
public:
	int message_id;
	std::optional<User> from; // may be empty from channels
	int date; // unix time
	Chat chat; 

	// optional
	std::optional<User> forward_from;
	std::optional<int> forward_date;
	std::unique_ptr<Message> reply_to_message;
	std::optional<std::string> text; // [0; 4096]
	// std::list<MessageEntity> entities;
	// Audio audio
	// Document document
	// std::list<PhotoSize> photo
	// Sticker sticker
	// Video video
	// Voice voice
	// std::string caption;
	// Contact contact
	// Location location
	// Venue venue
	// User new_char_member;
	// std::string new_chat_title;
	// std::list<PhotoSize> new_chat_photo

	std::unique_ptr<Message> pinned_message;
};
//void to_json(json& j, const Message& m) {
//	j = json{ 
//		{"name", p.name}, {"address", p.address}, {"age", p.age} 
//	};
//}

template<>
struct nlohmann::adl_serializer<Message> {
	static void from_json(const json& j, Message& m) {
		j.at("message_id").get_to(m.message_id);
		j.at("date").get_to(m.date);
		j.at("chat").get_to(m.chat);

		if (auto i = j.find("from"); i != j.end())
			m.from.emplace(i->get<User>());
		if (auto i = j.find("forward_from"); i != j.end())
			m.forward_from.emplace(i->get<User>());
		if (auto i = j.find("forward_date"); i != j.end())
			m.forward_date.emplace(i->get<int>());
		if (auto i = j.find("reply_to_message"); i != j.end())
			m.reply_to_message = std::make_unique<Message>(i->get<Message>());
		if (auto i = j.find("text"); i != j.end())
			m.text.emplace(i->get<std::string>());
		if (auto i = j.find("pinned_message"); i != j.end())
			m.pinned_message = std::make_unique<Message>(i->get<Message>());
	}
};
