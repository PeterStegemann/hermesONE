// Copyright 2008 Peter Stegemann

#include "Text.h"

#include "System.h"

const flash_char Text::ThreeDigitPercentFormat[] PROGMEM	= "%3d%%";
const flash_char Text::FourDigitPercentFormat[] PROGMEM		= "%4d%%";
const flash_char Text::EmptyDigitPercent[] PROGMEM			= "---%";
const flash_char Text::Int8Format[] PROGMEM					= "%d";
const flash_char Text::UInt8Format[] PROGMEM				= "%u";
const flash_char Text::FourDigitInt16Format[] PROGMEM		= "%4d";
const flash_char Text::SixDigitInt16Format[] PROGMEM		= "%6d";
const flash_char Text::CharacterFormat[] PROGMEM			= "%c";
const flash_char Text::TwinCharacterFormat[] PROGMEM		= "%c%c";
const flash_char Text::TimeFormat[] PROGMEM					= "%02u:%02u ";
const flash_char Text::NegativeTimeFormat[] PROGMEM			= "-%02u:%02u ";
const flash_char Text::VoltageFormat[] PROGMEM				= "%2u.%1uv ";
const flash_char Text::MillisecondFormat[] PROGMEM			= "%1u.%1u ms ";

const flash_char Text::Plus100Percent[] PROGMEM				= "100%";
const flash_char Text::PaddedZeroPercent[] PROGMEM			= "  0%";
const flash_char Text::Minus100Percent[] PROGMEM			= "-100%";
const flash_char Text::ZeroPercent[] PROGMEM				= "0%";
const flash_char Text::PaddedOff[] PROGMEM					= "  Aus";

const flash_char Text::hermesONE[] PROGMEM					= "hermesONE";

const flash_char Text::Add[] PROGMEM						= "Hinzufuegen";
const flash_char Text::Analog[] PROGMEM						= "Analog";
const flash_char Text::Backlight[] PROGMEM					= "Licht";
const flash_char Text::Battery[] PROGMEM					= "Batterie";
const flash_char Text::Button[] PROGMEM						= "Taster";
const flash_char Text::BlankTime[] PROGMEM					= "Bildschirm Aus";
const flash_char Text::Calibration[] PROGMEM				= "Kalibrierung";
const flash_char Text::Cancel[] PROGMEM						= "Abbrechen";
const flash_char Text::Channel[] PROGMEM					= "Kanal";
const flash_char Text::ChannelMapping[] PROGMEM				= "Kanalzuweisung";
const flash_char Text::Channels[] PROGMEM					= "Kanaele";
const flash_char Text::Clip[] PROGMEM						= "Schnitt";
const flash_char Text::Contrast[] PROGMEM					= "Kontrast";
const flash_char Text::Critical[] PROGMEM					= "Kritisch";
const flash_char Text::Follower[] PROGMEM					= "Folger";
const flash_char Text::Followers[] PROGMEM					= "Folger";
const flash_char Text::Delete[] PROGMEM						= "Loeschen";
const flash_char Text::Display[] PROGMEM					= "Display";
const flash_char Text::Empty[] PROGMEM						= "Leer";
const flash_char Text::Exit[] PROGMEM						= "Zurueck";
const flash_char Text::Fixed[] PROGMEM						= "Fest";
const flash_char Text::Global[] PROGMEM						= "Global";
const flash_char Text::Info[] PROGMEM						= "Info";
const flash_char Text::Init[] PROGMEM						= "Startwert";
const flash_char Text::Input[] PROGMEM						= "Geber";
const flash_char Text::InputA[] PROGMEM						= "Geber Oben";
const flash_char Text::InputB[] PROGMEM						= "Geber Unten";
const flash_char Text::Inputs[] PROGMEM						= "Geber";
const flash_char Text::Inverted[] PROGMEM					= "Invertiert";
const flash_char Text::Limit[] PROGMEM						= "Limit";
const flash_char Text::Low[] PROGMEM						= "Niedrig";
const flash_char Text::Main[] PROGMEM						= "Main";
const flash_char Text::Map[] PROGMEM						= "Kurve";
const flash_char Text::Maps[] PROGMEM						= "Kurven";
const flash_char Text::Maximum[] PROGMEM					= "Maximum";
const flash_char Text::Minimum[] PROGMEM					= "Minimum";
const flash_char Text::Mix[] PROGMEM						= "Mischer";
const flash_char Text::Mixers[] PROGMEM						= "Mischer";
const flash_char Text::Mode[] PROGMEM						= "Modus";
const flash_char Text::ModeWarp[] PROGMEM					= "Kurve";
const flash_char Text::ModeClip[] PROGMEM					= "Abschneiden";
const flash_char Text::Model[] PROGMEM						= "Modell";
const flash_char Text::Models[] PROGMEM						= "Modelle";
const flash_char Text::Name[] PROGMEM						= "Name";
const flash_char Text::None[] PROGMEM						= "Keine Quelle";
const flash_char Text::Ok[] PROGMEM							= "Ok";
const flash_char Text::Owner[] PROGMEM						= "Besitzer";
const flash_char Text::PPM[] PROGMEM						= "PPM";
const flash_char Text::Proxies[] PROGMEM					= "Proxies";
const flash_char Text::Proxy[] PROGMEM						= "Proxy";
const flash_char Text::Range[] PROGMEM						= "Bereich";
const flash_char Text::Reset[] PROGMEM						= "Reset";
const flash_char Text::ResetCheck[] PROGMEM					= "Alle Einstellungen zuruecksetzen?";
const flash_char Text::Reverse[] PROGMEM					= "Umkehr";
const flash_char Text::RFEnabled[] PROGMEM					= " HF Aktiviert ";
const flash_char Text::RFDisabled[] PROGMEM					= "HF Deaktiviert";
const flash_char Text::RFMode[] PROGMEM						= "HF Modul";
const flash_char Text::RFNone[] PROGMEM						= "Keins";
const flash_char Text::RFModule0[] PROGMEM					= "Modul 1";
const flash_char Text::RFModule1[] PROGMEM					= "Modul 2";
const flash_char Text::RFBoth[] PROGMEM						= "Beide";
const flash_char Text::Rotary[] PROGMEM						= "Drehgeber";
const flash_char Text::Serial[] PROGMEM						= "Serielle Verbindung";
const flash_char Text::SerialShort[] PROGMEM				= "Seriell";
const flash_char Text::SerialAsk[] PROGMEM					= "Serielle Verbindung aktiv...";
const flash_char Text::Setup[] PROGMEM						= "Einstellung";
const flash_char Text::Slot[] PROGMEM						= "Slot";
const flash_char Text::Source[] PROGMEM						= "Quelle";
const flash_char Text::Sources[] PROGMEM					= "Alle Quellen";
const flash_char Text::Status[] PROGMEM						= "Status";
const flash_char Text::StatusInverted[] PROGMEM				= "Invertieren";
const flash_char Text::Store[] PROGMEM						= "Speicher";
const flash_char Text::Stores[] PROGMEM						= "Speicher";
const flash_char Text::Step[] PROGMEM						= "Schrittweite";
const flash_char Text::Stop[] PROGMEM						= "Stop";
const flash_char Text::Switch[] PROGMEM						= "Schalter";
const flash_char Text::System[] PROGMEM						= "System";
const flash_char Text::SystemTime[] PROGMEM					= "Systemzeit";
const flash_char Text::Target[] PROGMEM						= "Soll";
const flash_char Text::Ticker[] PROGMEM						= "Ticker";
const flash_char Text::Time[] PROGMEM						= "Uhr";
const flash_char Text::Timer[] PROGMEM						= "Uhr";
const flash_char Text::Timers[] PROGMEM						= "Uhren";
const flash_char Text::Toggle[] PROGMEM						= "Wechsel";
const flash_char Text::Trigger[] PROGMEM					= "Trigger";
const flash_char Text::TriggerLowLimit[] PROGMEM			= "Limit Unten";
const flash_char Text::TriggerHighLimit[] PROGMEM			= "Oben";
const flash_char Text::Trim[] PROGMEM						= "Trimm";
const flash_char Text::Trimmer[] PROGMEM					= "Trimmer";
const flash_char Text::Trimmers[] PROGMEM					= "Trimmer";
const flash_char Text::Type[] PROGMEM						= "Typ";
const flash_char Text::Types[] PROGMEM						= "Typen";
const flash_char Text::Unknown[] PROGMEM					= "unbekannt";
const flash_char Text::Volume[] PROGMEM						= "Volumen";
const flash_char Text::Warn[] PROGMEM						= "Warnung";
const flash_char Text::WarnCritical[] PROGMEM				= "Kritisch";
const flash_char Text::WarnLow[] PROGMEM					= "Warnung Niedrig";
const flash_char Text::WarnPause[] PROGMEM					= "Warnung Pause";
const flash_char Text::Warp[] PROGMEM						= "Kurve";

const flash_char Text::Yes[] PROGMEM						= "Ja";
const flash_char Text::No[] PROGMEM							= "Nein";

const flash_char Text::Bottom[] PROGMEM						= "Unten";
const flash_char Text::Center[] PROGMEM						= "Mitte";
const flash_char Text::Top[] PROGMEM						= "Oben";

const flash_char Text::ChannelNameFormat[] PROGMEM			= "Kanal %d";
const flash_char Text::PPMNameFormat[] PROGMEM				= "Modul %d";
const flash_char Text::FollowerNameFormat[] PROGMEM			= "Folger %d";
const flash_char Text::InputNameFormat[] PROGMEM			= "Geber %d";
const flash_char Text::MapNameFormat[] PROGMEM				= "Kurve %d";
const flash_char Text::MixNameFormat[] PROGMEM				= "Mischer %d";
const flash_char Text::ProxyNameFormat[] PROGMEM			= "Proxy %d";
const flash_char Text::StoreNameFormat[] PROGMEM			= "Speicher %d";
const flash_char Text::TimerNameFormat[] PROGMEM			= "Uhr %d";
const flash_char Text::TrimmerNameFormat[] PROGMEM			= "Trimmer %d";
const flash_char Text::ModelNameFormat[] PROGMEM			= "Modell %d";
const flash_char Text::TypeNameFormat[] PROGMEM				= "Typ %d";

const flash_char Text::StatusUsageFormat[] PROGMEM			= "Modelle %u/%u Quellen %u/%u";

const flash_char Text::CantDeleteCurrentModel[] PROGMEM		= "Das Modell kann nicht geloescht werden!";
const flash_char Text::CantDeleteCurrentType[] PROGMEM		= "Der Typ kann nicht geloescht werden!";
const flash_char Text::SwitchTypeWarning[] PROGMEM			= "Wirklich den Typ wechseln?";

const flash_char Text::DeleteModelFormat[] PROGMEM			= "Model \"%s\" loeschen?";
const flash_char Text::DeleteSourceFollowerFormat[] PROGMEM	= "Folger \"%s\" loeschen?";
const flash_char Text::DeleteSourceInputFormat[] PROGMEM	= "Geber \"%s\" loeschen?";
const flash_char Text::DeleteSourceMapFormat[] PROGMEM		= "Kurve \"%s\" loeschen?";
const flash_char Text::DeleteSourceMixFormat[] PROGMEM		= "Mischer \"%s\" loeschen?";
const flash_char Text::DeleteSourceProxyFormat[] PROGMEM	= "Proxy \"%s\" loeschen?";
const flash_char Text::DeleteSourceStoreFormat[] PROGMEM	= "Speicher \"%s\" loeschen?";
const flash_char Text::DeleteSourceUnknownFormat[] PROGMEM	= "Quelle \"%s\" loeschen?";
const flash_char Text::DeleteSourceTimerFormat[] PROGMEM	= "Uhr \"%s\" loeschen?";
const flash_char Text::DeleteSourceTrimmerFormat[] PROGMEM	= "Trimmer \"%s\" loeschen?";
const flash_char Text::DeleteTypeFormat[] PROGMEM			= "Typ \"%s\" loeschen?";

const flash_char Text::NoProxyId[] PROGMEM					= "Kein Proxy verfuegbar!";
const flash_char Text::NoModelStorage[] PROGMEM				= "Kein Modellspeicher verfuegbar!";
const flash_char Text::NoSystemStorage[] PROGMEM			= "Kein Systemspeicher verfuegbar!";

const flash_char Text::ModelsCount[] PROGMEM				= "Modelle:  %3u/%3u (%u)";
const flash_char Text::TypesCount[] PROGMEM					= "Typen:   %3u/%3u (%u)";
const flash_char Text::SourcesCount[] PROGMEM				= "Quellen: %4u/%4u (%u)";
const flash_char Text::InternalEEPROMSize[] PROGMEM			= "Internes EEPROM: %5u / %5u";
const flash_char Text::ExternalEEPROMSize[] PROGMEM			= "Externes EEPROM: %5u / %5lu";
const flash_char Text::CPUType[] PROGMEM					= "CPU: " SYSTEM_MCU;

const flash_char Text::LeftSide[] PROGMEM					= "Linke Seite";
const flash_char Text::LeftBottom[] PROGMEM					= "Links Unten";
const flash_char Text::RightSide[] PROGMEM					= "Rechte Seite";
const flash_char Text::RightBottom[] PROGMEM				= "Rechts Unten";
const flash_char Text::TimerTop[] PROGMEM					= "Uhr Oben";
const flash_char Text::TimerBottom[] PROGMEM				= "Uhr Unten";
