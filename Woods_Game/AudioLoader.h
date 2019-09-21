#ifndef AUDIO_LOADER_H
#define AUDIO_LOADER_H

#include <allegro5/allegro_audio.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <iostream>

enum AUDIO_TYPES {
	AUDIO_TYPE_MUSIC,
	AUDIO_TYPE_SFX
};

class AudioLoader
{
private:
	AudioLoader() {}
	std::map<std::string, ALLEGRO_SAMPLE *> sample_map;
public:
	AudioLoader(AudioLoader const&) = delete;
	void operator=(AudioLoader const&) = delete;
	static AudioLoader& get_instance();
	~AudioLoader();
	const static std::string full_filename(const std::string);
	const static std::string full_filename(const std::string filename, const std::string path_prefix);
	const bool sample_exists(const std::string filename);
	void load_sample(const std::string filename, const int audio_type);
	void load_sample(const std::string filename, const int audio_type, const std::string path_prefix);
	ALLEGRO_SAMPLE* get_sample(const std::string filename, const int audio_type);
	ALLEGRO_SAMPLE* get_sample(const std::string filename, const int audio_type, const std::string path_prefix);
};
#endif
