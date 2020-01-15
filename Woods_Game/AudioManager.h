#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#define NO_MUSIC "no_music"

#include "AudioLoader.h"
#include "FileManager.h"
#include "Configurations.h"
#include <allegro5/allegro_audio.h>

#include <set>

enum SOUND_KEYS {
	SOUND_KEY_DEFAULT,
	SOUND_KEY_PLAYER,
	SOUND_KEY_NPC,
	SOUND_KEY_DIALOG,
	SOUND_KEY_JINGLE,
	SOUND_KEY_MENU
};

struct AudioInstance {
	ALLEGRO_SAMPLE_INSTANCE *instance;
	int duration = 0;
	AudioInstance(ALLEGRO_SAMPLE_INSTANCE * inst);
	bool is_empty = false;
};

class AudioManager
{
private:
	AudioManager() {}
	ALLEGRO_VOICE * master_voice = NULL;
	ALLEGRO_MIXER * master_mixer = NULL;
	ALLEGRO_MIXER * music_mixer = NULL;
	ALLEGRO_MIXER * sfx_mixer = NULL;
	std::map<std::pair<std::string, std::string>, AudioInstance *> sample_instance_map;
	std::set<std::string> active_music_tracks;
	
	AudioInstance * get_sample_instance(const std::string filename, const int audio_type);
	AudioInstance * get_sample_instance(const std::string filename, const int audio_type, const std::string sound_key);
	void load_sample_instance(const std::string filename, const int audio_type, const std::string sound_key);
	void play_audio(const std::string filename, const int audio_type);
	void play_audio(const std::string filename, const int audio_type, const std::string sound_key);
	void play_audio(const std::string filename, const int audio_type, const std::string sound_key, const float gain, const float pan, const bool stop_if_playing);
	const bool audio_exists(const std::string filename, const int audio_type, const std::string sound_key);
	void stop_all_music();
public:
	AudioManager(AudioManager const&) = delete;
	void operator=(AudioManager const&) = delete;
	static AudioManager& get_instance();
	~AudioManager();
	void initialize_audio();
	void play_music(const std::string filename);
	void play_sfx(const std::string filename, const std::string sound_key);
	void play_sfx(const std::string filename, const std::string sound_key, const float gain, const float pan, const bool stop_if_playing);
	const bool sfx_exists(const std::string filename, const std::string sound_key);
	void set_master_gain(const float gain);
	void set_music_gain(const float gain);
	void set_sfx_gain(const float gain);
	void play_menu_scroll_sound();
	
};

#endif