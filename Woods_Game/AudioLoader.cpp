#include "AudioLoader.h"

AudioLoader &AudioLoader::get_instance()
{
	static AudioLoader loader;
	return loader;
}

AudioLoader::~AudioLoader()
{
}

const std::string AudioLoader::full_filename(const std::string filename)
{
	return AudioLoader::get_instance().full_filename(filename, "resources/audio/");
}

const std::string AudioLoader::full_filename(const std::string filename, const std::string path_prefix)
{
	return path_prefix + filename + ".ogg"; //TODO: what file type? should it be passed in?
}

const bool AudioLoader::sample_exists(const std::string filename)
{
	const std::string full_filename = AudioLoader::full_filename(filename);
	return boost::filesystem::exists(full_filename.c_str());
}

void AudioLoader::load_sample(const std::string filename, const int audio_type)
{
	std::string prefix = "resources/audio/";
	switch (audio_type) {
	case AUDIO_TYPE_MUSIC:
		prefix = prefix + "music/";
		break;
	case AUDIO_TYPE_SFX:
		prefix = prefix + "sfx/";
		break;
	default:
		break;
	}
	AudioLoader::get_instance().load_sample(filename, audio_type, prefix);
}

void AudioLoader::load_sample(const std::string filename, const int audio_type, const std::string path_prefix)
{
	const std::string full_filename = AudioLoader::full_filename(filename, path_prefix);
	if (sample_map.find(full_filename) == sample_map.end()) {
		ALLEGRO_SAMPLE *sample = al_load_sample(full_filename.c_str());
		if (sample == NULL) {
			//TODO: error handling
			//TODO: sholud probably mark it as failed so we don't keep trying indefinitely
			std::cout << "loader failed to load sample " << filename << std::endl;
		} else {
			sample_map[full_filename] = sample;
		}
	}
}

ALLEGRO_SAMPLE * AudioLoader::get_sample(const std::string filename, const int audio_type)
{
	std::string prefix = "resources/audio/";
	switch (audio_type) {
	case AUDIO_TYPE_MUSIC:
		prefix = prefix + "music/";
		break;
	case AUDIO_TYPE_SFX:
		prefix = prefix + "sfx/";
		break;
	default:
		break;
	}
	return AudioLoader::get_instance().get_sample(filename, audio_type, prefix);
}

ALLEGRO_SAMPLE * AudioLoader::get_sample(const std::string filename, const int audio_type, const std::string path_prefix)
{
	const std::string full_filename = AudioLoader::full_filename(filename, path_prefix);
	if (sample_map.find(full_filename) == sample_map.end()) {
		AudioLoader::get_instance().load_sample(filename, audio_type, path_prefix);
	}
	if (sample_map.find(full_filename) != sample_map.end()) {
		return sample_map[full_filename];
	}
	//TODO: error handling
	return NULL;
}
