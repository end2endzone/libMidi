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

// This class is exported from the libMidi.dll
class LIBMIDI_API MidiFile {
public:
	MidiFile(void);
  void addNote(unsigned short iFrequency, unsigned short iDurationMs);
  void setTicksPerQuarterNotes(unsigned short iTicks);
  void setBeatsPerMinutes(unsigned short iBpm);
  void setName(const char * iName);
  bool save(const char * iFile);
	// TODO: add your methods here.

private:
  struct NOTE
  {
    unsigned short frequency;
    unsigned short durationMs;
  };
  typedef std::vector<NOTE> NoteList;
  unsigned short mTicksPerQuarterNote;
  std::string mName;
  NoteList mNotes;
};
