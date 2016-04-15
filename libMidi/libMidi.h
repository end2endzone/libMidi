// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBMIDI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBMIDI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBMIDI_EXPORTS
#define LIBMIDI_API __declspec(dllexport)
#else
#define LIBMIDI_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include "miditypes.h"

// This class is exported from the libMidi.dll
class LIBMIDI_API MidiFile {
public:
	MidiFile(void);
  void addNote(uint16_t iFrequency, uint16_t iDurationMs);
  void setTicksPerQuarterNote(uint16_t iTicks);
  void setBeatsPerMinute(uint16_t iBpm);
  void setTempo(uint32_t iTempo); //usec per quarter note
  void setName(const char * iName);
  //void computeAutoTempo();
  uint16_t computeTicks(uint16_t iDurationMs);
  bool save(const char * iFile);

public:
  static const uint16_t DEFAULT_BPM = 120;
  static const uint32_t DEFAULT_TEMPO = 500000;
  static const uint32_t DEFAULT_TICKS_PER_QUARTER_NOTE = 480;
  static const uint32_t MIN2USEC = 60*1000*1000;

private:
  struct NOTE
  {
    uint16_t frequency;
    uint16_t durationMs;
  };
  typedef std::vector<NOTE> NoteList;
  uint16_t mTicksPerQuarterNote;
  uint32_t mTempo; //usec per quarter note
  std::string mName;
  NoteList mNotes;
};
