#include "AudioManager.h"

AudioManager & AudioManager::get_instance()
{
	static AudioManager manager;
	return manager;
}

AudioManager::~AudioManager()
{
}

void AudioManager::initialize_audio()
{
	//TODO: load serialized configurations like master volume
	master_voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	master_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	music_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	sfx_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

	//al_set_default_mixer(master_mixer);
	
	al_attach_mixer_to_mixer(music_mixer, master_mixer);
	al_attach_mixer_to_mixer(sfx_mixer, master_mixer);
	al_attach_mixer_to_voice(master_mixer, master_voice);

	const std::string config_path = "resources/config";
	const std::string config_key = "current_configurations";
	FileManager filemanager;
	Configurations config;
	filemanager.load_xml_content(&config, config_path, "SerializableClass", "ConfigurationsKey", config_key);
	
	const float master_volume = config.get_master_volume();
	const float music_volume = config.get_master_volume();
	const float sfx_volume = config.get_master_volume();

	al_set_mixer_gain(master_mixer, master_volume / 100.0);
	al_set_mixer_gain(music_mixer, music_volume / 100.0);
	al_set_mixer_gain(sfx_mixer, sfx_volume / 100.0);

	al_set_mixer_playing(master_mixer, true);
}

ALLEGRO_SAMPLE_INSTANCE * AudioManager::get_sample_instance(const std::string filename, const int audio_type)
{
	return this->get_sample_instance(filename, audio_type, "" + SOUND_KEY_DEFAULT);
}

ALLEGRO_SAMPLE_INSTANCE * AudioManager::get_sample_instance(const std::string filename, const int audio_type, const std::string sound_key)
{
	const std::pair<std::string, std::string> key_pair(filename, sound_key);
	if (sample_instance_map.find(key_pair) == sample_instance_map.end()) {
		AudioManager::get_instance().load_sample_instance(filename, audio_type, sound_key);
	}
	if (sample_instance_map.find(key_pair) != sample_instance_map.end()) {
		return sample_instance_map[key_pair];
	}
	//TODO: error handling
	return NULL;
}

void AudioManager::load_sample_instance(const std::string filename, const int audio_type, const std::string sound_key)
{
	const std::pair<std::string, std::string> key_pair(filename, sound_key);
	if (sample_instance_map.find(key_pair) == sample_instance_map.end()) {
		ALLEGRO_MIXER * mixer = NULL;
		switch (audio_type) {
		case AUDIO_TYPE_MUSIC:
			mixer = music_mixer;
			break;
		case AUDIO_TYPE_SFX:
			mixer = sfx_mixer;
			break;
		default:
			break;
		}
		ALLEGRO_SAMPLE * sample = AudioLoader::get_instance().get_sample(filename, audio_type);
		if (sample != NULL) {
			//TODO: do we need to key by audio type?
			ALLEGRO_SAMPLE_INSTANCE * sample_instance = al_create_sample_instance(sample);
			sample_instance_map[key_pair] = sample_instance;
			al_attach_sample_instance_to_mixer(sample_instance, mixer);
		}
	}
}

void AudioManager::play_music(const std::string filename)
{
	play_audio(filename, AUDIO_TYPE_MUSIC);
}

void AudioManager::play_sfx(const std::string filename, const std::string sound_key)
{
	play_audio(filename, AUDIO_TYPE_SFX, sound_key);
}

void AudioManager::play_sfx(const std::string filename, const std::string sound_key, const float gain, const float pan)
{
	play_audio(filename, AUDIO_TYPE_SFX, sound_key, gain, pan);
}

const bool AudioManager::sfx_exists(const std::string filename, const std::string sound_key)
{
	return this->audio_exists(filename, AUDIO_TYPE_SFX, sound_key);
}

const bool AudioManager::audio_exists(const std::string filename, const int audio_type, const std::string sound_key)
{
	ALLEGRO_SAMPLE_INSTANCE * instance = this->get_sample_instance(filename, audio_type, sound_key);
	return instance != NULL;
}

void AudioManager::set_master_gain(const float gain)
{
	al_set_mixer_gain(master_mixer, gain);
}

void AudioManager::set_music_gain(const float gain)
{
	al_set_mixer_gain(music_mixer, gain);
}

void AudioManager::set_sfx_gain(const float gain)
{
	al_set_mixer_gain(sfx_mixer, gain);
}

void AudioManager::play_audio(const std::string filename, const int audio_type)
{
	this->play_audio(filename, audio_type, "" + SOUND_KEY_DEFAULT);
}

void AudioManager::play_audio(const std::string filename, const int audio_type, const std::string sound_key)
{
	//TODO: do we need to do anything special when restarting audio that is/was already playing?

	ALLEGRO_SAMPLE_INSTANCE * sample_instance = AudioManager::get_instance().get_sample_instance(filename, audio_type, sound_key);

	if (al_play_sample_instance(sample_instance)) {
		// success
	}
	else {
		// failure
	}
}

void AudioManager::play_audio(const std::string filename, const int audio_type, const std::string sound_key, const float gain, const float pan)
{
	ALLEGRO_SAMPLE_INSTANCE * sample_instance = AudioManager::get_instance().get_sample_instance(filename, audio_type, sound_key);
	
	al_set_sample_instance_gain(sample_instance, gain);
	al_set_sample_instance_pan(sample_instance, pan);

	if (al_play_sample_instance(sample_instance)) {
		// success
	}
	else {
		// failure
	}
}
