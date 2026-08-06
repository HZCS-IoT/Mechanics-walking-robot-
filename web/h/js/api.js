/*
 * api.js — ربط لوحة التحكم بـ MQTT → ESP32
 * Smart Methods — RoboDog BodyV2
 *
 * يُحمّل في manual.html و voice.html
 * يحتاج mqtt.min.js (CDN أو محلي)
 */

// ═══ إعدادات MQTT — يجب أن تطابق robodog_mqtt.ino ═══
const MQTT_BROKER = 'wss://broker.hivemq.com:8884/mqtt'; // WSS للمتصفح (HTTPS)
const MQTT_TOPIC = 'smartmethods/robodog/command';       // نفس Topic في ESP32

// ═══ تحويل اسم الزر/الصوت → حرف يرسل عبر MQTT ═══
const CMD_LETTER = {
  forward: 'f',       // أمام
  backward: 'b',      // خلف
  stop: 'S',          // وقوف
  wave_left: 'wL',    // مصافحة يسار
  wave_right: 'wR',   // مصافحة يمين
  sit: 'j'            // جلس
};

let mqttClient = null;
let mqttConnected = false;

// ─── تحديث شارة حالة MQTT في الصفحة ───
function setMqttBadge(text, ok) {
  const el = document.getElementById('mqttStatus');
  if (!el) return;
  el.textContent = text;
  el.style.color = ok ? '#22c55e' : '#f87171';
}

// ─── الاتصال بـ Broker (Promise — للانتظار قبل الإرسال) ───
function initMqtt() {
  return new Promise(function (resolve, reject) {
    if (typeof mqtt === 'undefined') {
      setMqttBadge('MQTT: mqtt library missing', false);
      reject(new Error('mqtt library missing'));
      return;
    }

    if (mqttClient && mqttClient.connected) {
      mqttConnected = true;
      setMqttBadge('MQTT: متصل ✓', true);
      resolve();
      return;
    }

    setMqttBadge('MQTT: يتصل...', false);

    mqttClient = mqtt.connect(MQTT_BROKER, {
      clientId: 'phone_' + Math.random().toString(16).slice(2, 10),
      clean: true,
      reconnectPeriod: 3000,
      connectTimeout: 20000,
      protocolVersion: 4
    });

    var done = false;
    var timer = setTimeout(function () {
      if (done) return;
      done = true;
      setMqttBadge('MQTT: فشل الاتصال', false);
      reject(new Error('MQTT timeout'));
    }, 20000);

    mqttClient.on('connect', function () {
      if (done) return;
      done = true;
      clearTimeout(timer);
      mqttConnected = true;
      setMqttBadge('MQTT: متصل ✓', true);
      resolve();
    });

    mqttClient.on('reconnect', function () {
      setMqttBadge('MQTT: يعيد الاتصال...', false);
    });

    mqttClient.on('close', function () {
      mqttConnected = false;
      setMqttBadge('MQTT: انقطع', false);
    });

    mqttClient.on('error', function (err) {
      if (done) return;
      done = true;
      clearTimeout(timer);
      mqttConnected = false;
      setMqttBadge('MQTT: خطأ', false);
      reject(err);
    });
  });
}

// ─── نشر حرف واحد على Topic ───
function publishMqtt(command) {
  var letter = CMD_LETTER[command];
  if (!letter) return Promise.reject(new Error('unknown command'));

  return initMqtt().then(function () {
    return new Promise(function (resolve, reject) {
      mqttClient.publish(MQTT_TOPIC, letter, { qos: 0 }, function (err) {
        if (err) reject(err);
        else resolve(letter);
      });
    });
  });
}

// ─── الدالة الرئيسية — تُستدعى من الأزرار أو الصوت ───
async function sendRobotCommand(command) {
  var letter;
  try {
    letter = await publishMqtt(command);
  } catch (e) {
    throw new Error('MQTT ما وصل: ' + (e.message || e));
  }

  // اختياري: حفظ في PHP إن وُجد على الاستضافة (لا يؤثر على ESP32)
  try {
    await fetch('update_command.php?command=' + encodeURIComponent(command), {
      method: 'GET',
      credentials: 'same-origin'
    });
  } catch (e) {
    // تجاهل — المهم MQTT
  }

  return {
    status: 'success',
    button: command,
    stored_as: letter,
    mqtt: true,
    message: 'MQTT وصل للESP32: ' + letter + ' (' + command + ')'
  };
}

// ─── حفظ نص الصوت (اختياري — voice.html) ───
async function saveSpeechText(text) {
  var response = await fetch(
    'save_speech.php?text=' + encodeURIComponent(text),
    { method: 'GET', credentials: 'same-origin' }
  );
  var data = JSON.parse(await response.text());
  if (!response.ok || data.status !== 'success') {
    throw new Error(data.message || 'فشل حفظ النص');
  }
  return data;
}

// ─── تحديث عنصر status في الصفحة ───
function setStatus(element, text, type) {
  element.textContent = text;
  element.className = 'status' + (type ? ' ' + type : '');
}

// ─── محاولة اتصال MQTT عند فتح الصفحة ───
document.addEventListener('DOMContentLoaded', function () {
  initMqtt().catch(function () {});
});
