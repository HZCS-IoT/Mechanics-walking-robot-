/*
 * voice-commands.js — تحويل الكلام إلى أمر روبوت
 * Smart Methods — RoboDog BodyV2
 *
 * يُستخدم في voice.html مع api.js
 * parseVoiceCommand(text) → 'forward' | 'wave_right' | ... | null
 */

const COMMAND_LABELS = {
  forward: 'للأمام',
  backward: 'للخلف',
  stop: 'توقف',
  wave_left: 'تلويح يسار',
  wave_right: 'تلويح / سلم يمين',
  sit: 'اجلس',
};

const RECOGNITION_LANG = {
  ar: 'ar-SA',
  en: 'en-US',
};

const ARABIC_TRANSCRIPTIONS = {
  'ستوب': 'stop',
  'ستوپ': 'stop',
  'فوروارد': 'forward',
  'فورورد': 'forward',
  'فور وورد': 'forward',
  'فور': 'forward',
  'باكورد': 'backward',
  'باك وورد': 'backward',
  'باك': 'backward',
  'ويف': 'wave',
  'ويف رايت': 'wave right',
  'ويف لفت': 'wave left',
  'شيك': 'shake',
  'هلو': 'hello',
  'هاي': 'hi',
};

const VOICE_RULES = [
  {
    command: 'stop',
    keywords: [
      'stop', 'stopped', 'halt', 'stand still', 'stand',
      'توقف', 'قف', 'وقف', 'توقيف', 'قف بسرعة', 'توقف الان', 'توقف الآن',
      'ستوب', 'ستوپ', 'وقوف', 'ثبت',
    ],
  },
  {
    command: 'sit',
    keywords: [
      'sit', 'sit down', 'sitting',
      'اجلس', 'اجلسي', 'قعد', 'قاعد', 'انحناء', 'انحني',
    ],
  },
  {
    command: 'wave_left',
    keywords: [
      'wave left', 'left wave', 'shake left', 'left hand',
      'wave with left', 'wave on the left',
      'تلويح يسار', 'تلويح باليسار', 'صافح يسار', 'صافح باليسار',
      'سلم يسار', 'سلم باليسار', 'اليد اليسار', 'اليد اليسرى',
      'wave يسار', 'wave باليسار',
    ],
  },
  {
    command: 'wave_right',
    keywords: [
      'wave right', 'right wave', 'shake hands', 'shake hand', 'handshake',
      'say hi', 'say hello', 'hello', 'hi there', 'wave hello',
      'wave with right', 'wave on the right', 'right hand',
      'salam', 'salaam', 'assalam', 'assalamu',
      'سلم', 'سلام', 'صافح', 'صافحة', 'صافحني', 'صافحني',
      'صافح باليمين', 'سلم باليمين', 'تلويح يمين', 'تلويح باليمين',
      'اليد اليمنى', 'اليد اليمين', 'مرحبا', 'اهلا', 'أهلا',
      'wave', 'wave hand', 'wave hi',
    ],
  },
  {
    command: 'forward',
    keywords: [
      'forward', 'forwards', 'go forward', 'move forward',
      'امام', 'الامام', 'للامام', 'للأمام', 'قدام', 'للقدام', 'تقدم', 'روح قدام', 'تحرك للامام',
      'فوروارد', 'فورورد', 'فور', 'فرورد',
    ],
  },
  {
    command: 'backward',
    keywords: [
      'backward', 'back', 'go back', 'move back',
      'خلف', 'الخلف', 'للخلف', 'ورا', 'لورا', 'تراجع', 'ارجع', 'ارجعي',
      'باكورد', 'باك',
    ],
  },
];

// ─── تنظيف النص قبل المطابقة ───
function normalizeSpeech(text) {
  return text
    .trim()
    .toLowerCase()
    .replace(/[ًٌٍَُِّْ]/g, '')
    .replace(/[^\u0600-\u06FFa-z0-9\s]/gi, ' ')
    .replace(/\s+/g, ' ');
}

// ─── استبدال كلمات transliteration ───
function applyTransliterations(text) {
  let result = text;

  const sorted = Object.entries(ARABIC_TRANSCRIPTIONS).sort(
    (a, b) => b[0].length - a[0].length
  );

  for (const [arabic, english] of sorted) {
    const key = normalizeSpeech(arabic);
    if (key && result.indexOf(key) !== -1) {
      result = result.split(key).join(english);
    }
  }

  return result;
}

// ─── البحث في VOICE_RULES ───
function parseVoiceCommand(text) {
  const normalized = applyTransliterations(normalizeSpeech(text));

  for (const rule of VOICE_RULES) {
    for (const keyword of rule.keywords) {
      const key = normalizeSpeech(keyword);
      if (key && normalized.includes(key)) {
        return rule.command;
      }
    }
  }

  return null;
}

function getCommandLabel(command) {
  return COMMAND_LABELS[command] || command;
}

function getRecognitionLang(mode) {
  return RECOGNITION_LANG[mode] || RECOGNITION_LANG.ar;
}

function getLangHint(mode) {
  if (mode === 'en') {
    return 'English: forward, backward, stop, wave right, wave left, shake hands, sit';
  }
  return 'عربي: للأمام، للخلف، قف، سلم / صافح (يمين)، صافح يسار، اجلس';
}
