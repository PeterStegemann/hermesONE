// Copyright 2008 Peter Stegemann

#include "Text.h"

#include "System.h"

const flash_char Text::ThreeDigitPercentFormat[] PROGMEM    = "%3d%%";
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
const flash_char Text::PaddedOff[] PROGMEM					= "  Off";

const flash_char Text::hermesONE[] PROGMEM					= "hermesONE";

const flash_char Text::Add[] PROGMEM						= "Add";
const flash_char Text::Analog[] PROGMEM						= "Analog";
const flash_char Text::Backlight[] PROGMEM					= "Backlight";
const flash_char Text::Battery[] PROGMEM					= "Battery";
const flash_char Text::BlankTime[] PROGMEM					= "Blank Time";
const flash_char Text::Button[] PROGMEM						= "Button";
const flash_char Text::Calibration[] PROGMEM				= "Calibration";
const flash_char Text::Cancel[] PROGMEM						= "Cancel";
const flash_char Text::Channel[] PROGMEM					= "Channel";
const flash_char Text::ChannelMapping[] PROGMEM				= "Channelmapping";
const flash_char Text::Channels[] PROGMEM					= "Channels";
const flash_char Text::Clip[] PROGMEM						= "Clip";
const flash_char Text::Contrast[] PROGMEM					= "Contrast";
const flash_char Text::Critical[] PROGMEM					= "Critical";
const flash_char Text::Follower[] PROGMEM					= "Follower";
const flash_char Text::Followers[] PROGMEM					= "Followers";
const flash_char Text::Delete[] PROGMEM						= "Delete";
const flash_char Text::Display[] PROGMEM					= "Display";
const flash_char Text::Empty[] PROGMEM						= "Empty";
const flash_char Text::Exit[] PROGMEM						= "Exit";
const flash_char Text::Fixed[] PROGMEM						= "Fixed";
const flash_char Text::Global[] PROGMEM						= "Global";
const flash_char Text::Info[] PROGMEM						= "Info";
const flash_char Text::Init[] PROGMEM						= "Initial";
const flash_char Text::Input[] PROGMEM						= "Input";
const flash_char Text::InputA[] PROGMEM						= "Input High";
const flash_char Text::InputB[] PROGMEM						= "Input Low";
const flash_char Text::Inputs[] PROGMEM						= "Inputs";
const flash_char Text::Inverted[] PROGMEM					= "Inverted";
const flash_char Text::Limit[] PROGMEM						= "Limit";
const flash_char Text::Low[] PROGMEM						= "Low";
const flash_char Text::Main[] PROGMEM						= "Main";
const flash_char Text::Map[] PROGMEM						= "Map";
const flash_char Text::Maps[] PROGMEM						= "Maps";
const flash_char Text::Maximum[] PROGMEM					= "Maximum";
const flash_char Text::Minimum[] PROGMEM					= "Minimum";
const flash_char Text::Mix[] PROGMEM						= "Mix";
const flash_char Text::Mixers[] PROGMEM						= "Mixers";
const flash_char Text::Mode[] PROGMEM						= "Mode";
const flash_char Text::ModeWarp[] PROGMEM					= "Warp";
const flash_char Text::ModeClip[] PROGMEM					= "Clip";
const flash_char Text::Model[] PROGMEM						= "Model";
const flash_char Text::Models[] PROGMEM						= "Models";
const flash_char Text::Name[] PROGMEM						= "Name";
const flash_char Text::None[] PROGMEM						= "No Source";
const flash_char Text::Ok[] PROGMEM							= "Ok";
const flash_char Text::Owner[] PROGMEM						= "Owner";
const flash_char Text::PPM[] PROGMEM						= "PPM";
const flash_char Text::Proxies[] PROGMEM					= "Proxies";
const flash_char Text::Proxy[] PROGMEM						= "Proxy";
const flash_char Text::Range[] PROGMEM						= "Range";
const flash_char Text::Reset[] PROGMEM						= "Reset";
const flash_char Text::ResetCheck[] PROGMEM					= "Reset all settings?";
const flash_char Text::Reverse[] PROGMEM					= "Reverse";
const flash_char Text::RFEnabled[] PROGMEM					= " RF Enabled";
const flash_char Text::RFDisabled[] PROGMEM					= "RF Disabled";
const flash_char Text::RFMode[] PROGMEM						= "RF Module";
const flash_char Text::RFNone[] PROGMEM						= "None";
const flash_char Text::RFModule0[] PROGMEM					= "Module 1";
const flash_char Text::RFModule1[] PROGMEM					= "Module 2";
const flash_char Text::RFBoth[] PROGMEM						= "Both";
const flash_char Text::Rotary[] PROGMEM						= "Rotary";
const flash_char Text::Serial[] PROGMEM						= "Serial";
const flash_char Text::SerialShort[] PROGMEM				= "Serial";
const flash_char Text::SerialAsk[] PROGMEM					= "Serial connection active...";
const flash_char Text::Setup[] PROGMEM						= "Setup";
const flash_char Text::SetupBacklight[] PROGMEM				= "Setup Backlight";
const flash_char Text::Slot[] PROGMEM						= "Slot";
const flash_char Text::Source[] PROGMEM						= "Source";
const flash_char Text::Sources[] PROGMEM					= "All Sources";
const flash_char Text::Status[] PROGMEM						= "Status";
const flash_char Text::StatusInverted[] PROGMEM				= "Invert";
const flash_char Text::Store[] PROGMEM						= "Store";
const flash_char Text::Stores[] PROGMEM						= "Stores";
const flash_char Text::Stop[] PROGMEM						= "Stop";
const flash_char Text::Switch[] PROGMEM						= "Switch";
const flash_char Text::System[] PROGMEM						= "System";
const flash_char Text::SystemTime[] PROGMEM					= "System Time";
const flash_char Text::Target[] PROGMEM						= "Target";
const flash_char Text::Ticker[] PROGMEM						= "Ticker";
const flash_char Text::Time[] PROGMEM						= "Time";
const flash_char Text::Timer[] PROGMEM						= "Timer";
const flash_char Text::Timers[] PROGMEM						= "Timers";
const flash_char Text::Toggle[] PROGMEM						= "Toggle";
const flash_char Text::Trigger[] PROGMEM					= "Trigger";
const flash_char Text::TriggerLowLimit[] PROGMEM			= "Limit Low";
const flash_char Text::TriggerHighLimit[] PROGMEM			= "High";
const flash_char Text::Trim[] PROGMEM						= "Trim";
const flash_char Text::Timer[] PROGMEM						= "Trimmer";
const flash_char Text::Timers[] PROGMEM						= "Trimmers";
const flash_char Text::Type[] PROGMEM						= "Type";
const flash_char Text::Types[] PROGMEM						= "Types";
const flash_char Text::Unknown[] PROGMEM					= "unknown";
const flash_char Text::Volume[] PROGMEM						= "Volume";
const flash_char Text::Warn[] PROGMEM						= "Warn";
const flash_char Text::WarnCritical[] PROGMEM				= "Critical";
const flash_char Text::WarnLow[] PROGMEM					= "Warn Low";
const flash_char Text::WarnPause[] PROGMEM					= "Warn Pause";
const flash_char Text::Warp[] PROGMEM						= "Warp";

const flash_char Text::Yes[] PROGMEM						= "Yes";
const flash_char Text::No[] PROGMEM							= "No";

const flash_char Text::Bottom[] PROGMEM						= "Bottom";
const flash_char Text::Center[] PROGMEM						= "Center";
const flash_char Text::Top[] PROGMEM						= "Top";

const flash_char Text::ChannelNameFormat[] PROGMEM			= "Channel %u";
const flash_char Text::PPMNameFormat[] PROGMEM				= "Modul %u";
const flash_char Text::FollowerNameFormat[] PROGMEM			= "Follower %u";
const flash_char Text::InputNameFormat[] PROGMEM			= "Input %u";
const flash_char Text::MapNameFormat[] PROGMEM				= "Map %u";
const flash_char Text::MixNameFormat[] PROGMEM				= "Mix %u";
const flash_char Text::ProxyNameFormat[] PROGMEM			= "Proxy %u";
const flash_char Text::StoreNameFormat[] PROGMEM			= "Store %u";
const flash_char Text::TimerNameFormat[] PROGMEM			= "Timer %u";
const flash_char Text::TrimmerNameFormat[] PROGMEM			= "Trimmer %u";
const flash_char Text::ModelNameFormat[] PROGMEM			= "Model %u";
const flash_char Text::TypeNameFormat[] PROGMEM				= "Type %u";

const flash_char Text::StatusUsageFormat[] PROGMEM			= "Models %u/%u Sources %u/%u";

const flash_char Text::CantDeleteCurrentModel[] PROGMEM		= "Can't delete current model!";
const flash_char Text::CantDeleteCurrentType[] PROGMEM		= "Can't delete current type!";
const flash_char Text::SwitchTypeWarning[] PROGMEM			= "This can make your model unusable!";

const flash_char Text::DeleteModelFormat[] PROGMEM			= "Delete model \"%s\"?";
const flash_char Text::DeleteSourceFollowerFormat[] PROGMEM	= "Delete follower \"%s\"?";
const flash_char Text::DeleteSourceInputFormat[] PROGMEM	= "Delete input \"%s\"?";
const flash_char Text::DeleteSourceMapFormat[] PROGMEM		= "Delete map \"%s\"?";
const flash_char Text::DeleteSourceMixFormat[] PROGMEM		= "Delete mix \"%s\"?";
const flash_char Text::DeleteSourceProxyFormat[] PROGMEM	= "Delete proxy \"%s\"?";
const flash_char Text::DeleteSourceStoreFormat[] PROGMEM	= "Delete store \"%s\"?";
const flash_char Text::DeleteSourceTimerFormat[] PROGMEM	= "Delete timer \"%s\"?";
const flash_char Text::DeleteSourceTrimmerFormat[] PROGMEM	= "Delete trimmer \"%s\"?";
const flash_char Text::DeleteSourceUnknownFormat[] PROGMEM	= "Delete unknown \"%s\"?";
const flash_char Text::DeleteTypeFormat[] PROGMEM			= "Delete type \"%s\"?";

const flash_char Text::NoProxyId[] PROGMEM					= "No proxy available!";
const flash_char Text::NoModelStorage[] PROGMEM				= "No model storage available!";
const flash_char Text::NoSystemStorage[] PROGMEM			= "No system storage available!";

const flash_char Text::ModelsCount[] PROGMEM				= "Modelle: %3u / %2u  (%u)";
const flash_char Text::TypesCount[] PROGMEM					= "Typen:   %3u / %2u  (%u)";
const flash_char Text::SourcesCount[] PROGMEM				= "Quellen: %3u / %3u  (%u)";
const flash_char Text::InternalEEPROMSize[] PROGMEM			= "Internes EEPROM: %5u / %5u";
const flash_char Text::ExternalEEPROMSize[] PROGMEM			= "Externes EEPROM: %5u / %5lu";
const flash_char Text::CPUType[] PROGMEM					= "CPU: " SYSTEM_MCU_NAME;

const flash_char Text::LeftSide[] PROGMEM					= "Left Side";
const flash_char Text::LeftBottom[] PROGMEM					= "Left Bottom";
const flash_char Text::RightSide[] PROGMEM					= "Right Side";
const flash_char Text::RightBottom[] PROGMEM				= "Right Bottom";
const flash_char Text::TimerTop[] PROGMEM					= "Timer Top";
const flash_char Text::TimerBottom[] PROGMEM				= "Timer Bottom";
