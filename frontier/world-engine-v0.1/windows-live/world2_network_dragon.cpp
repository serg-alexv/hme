#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0601

#include <winsock2.h>
#include <ws2ipdef.h>
#include <windows.h>
#include <windowsx.h>
#include <bcrypt.h>
#include <objidl.h>
#include <gdiplus.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <shlobj.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cwchar>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "resources.h"

#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")

using namespace Gdiplus;

namespace {

constexpr wchar_t kAppTitle[] = L"World 2.0 - Live Network Pets";
constexpr UINT_PTR kTimerId = 1;
constexpr UINT kSampleMs = 250;
constexpr int kHistoryCount = 64;
constexpr std::int64_t kPpm = 1000000;
constexpr double kPi = 3.14159265358979323846;

enum class MotionState : std::uint8_t { Calm, Attune, Storm, Stale };
enum class SourceMode : std::uint8_t { Live, Replay };
enum class PetKind : std::uint8_t { Dragon, Equalizer, Field };

struct Quality {
    bool valid = false;
    bool counterReset = false;
    bool intervalStable = false;
    bool capacityDeclared = true;
};

struct WorldStateFrame {
    std::uint64_t sequence = 0;
    std::uint64_t observedAtMs = 0;
    std::uint64_t emittedAtMs = 0;
    std::uint64_t rawValueBps = 0;
    std::uint64_t rxBps = 0;
    std::uint64_t txBps = 0;
    std::uint64_t normalizationReferenceBps = 20000000;
    std::uint32_t sampleIntervalMs = kSampleMs;
    std::uint32_t ageMs = 0;
    std::int64_t normalizedValuePpm = 0;
    std::int64_t changePpm = 0;
    std::int64_t variabilityPpm = 0;
    std::int64_t noveltyPpm = 0;
    MotionState current = MotionState::Stale;
    MotionState previous = MotionState::Stale;
    std::uint32_t dwellFrames = 0;
    const char* reason = "NO_SAMPLE";
    std::int64_t intensityPpm = 0;
    std::int64_t energyPpm = 0;
    std::int64_t urgencyPpm = 0;
    Quality quality{};
    std::string payloadHash = "sha256:unavailable";
};

struct AppState {
    WorldStateFrame frame{};
    SourceMode sourceMode = SourceMode::Live;
    PetKind pet = PetKind::Dragon;
    bool alwaysOnTop = false;
    bool hasBaseline = false;
    ULONGLONG previousRx = 0;
    ULONGLONG previousTx = 0;
    LARGE_INTEGER previousClock{};
    LARGE_INTEGER clockFrequency{};
    std::array<std::int64_t, kHistoryCount> history{};
    std::array<bool, kHistoryCount> historyValid{};
    int historyCount = 0;
    int historyNext = 0;
    MotionState pendingState = MotionState::Stale;
    std::uint32_t pendingFrames = 0;
    unsigned interfaceCount = 0;
    double phase = 0.0;
    wchar_t error[180]{};
    wchar_t outputPath[MAX_PATH]{};
    Bitmap* dragonCalm = nullptr;
    Bitmap* dragonAttune = nullptr;
    Bitmap* dragonStorm = nullptr;
};

AppState gApp;
ULONG_PTR gGdiplusToken = 0;

std::int64_t clampPpm(std::int64_t value) {
    return std::clamp<std::int64_t>(value, 0, kPpm);
}

const wchar_t* stateName(MotionState state) {
    switch (state) {
        case MotionState::Calm: return L"CALM";
        case MotionState::Attune: return L"ATTUNE";
        case MotionState::Storm: return L"STORM";
        default: return L"STALE";
    }
}

const char* stateNameAscii(MotionState state) {
    switch (state) {
        case MotionState::Calm: return "CALM";
        case MotionState::Attune: return "ATTUNE";
        case MotionState::Storm: return "STORM";
        default: return "STALE";
    }
}

const wchar_t* petName(PetKind pet) {
    switch (pet) {
        case PetKind::Dragon: return L"WARRIOR DRAGON";
        case PetKind::Equalizer: return L"EQUALIZER";
        default: return L"FIELD";
    }
}

Color stateColor(MotionState state, BYTE alpha = 255) {
    switch (state) {
        case MotionState::Calm: return Color(alpha, 43, 226, 181);
        case MotionState::Attune: return Color(alpha, 255, 198, 87);
        case MotionState::Storm: return Color(alpha, 255, 77, 125);
        default: return Color(alpha, 125, 142, 163);
    }
}

std::uint64_t unixTimeMs() {
    FILETIME fileTime{};
    GetSystemTimeAsFileTime(&fileTime);
    ULARGE_INTEGER value{};
    value.LowPart = fileTime.dwLowDateTime;
    value.HighPart = fileTime.dwHighDateTime;
    return (value.QuadPart - UINT64_C(116444736000000000)) / UINT64_C(10000);
}

std::string sha256(const std::string& data) {
    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    DWORD objectLength = 0;
    DWORD hashLength = 0;
    DWORD copied = 0;
    std::string result = "unavailable";
    if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0) return result;
    if (BCryptGetProperty(algorithm, BCRYPT_OBJECT_LENGTH,
                          reinterpret_cast<PUCHAR>(&objectLength), sizeof(objectLength), &copied, 0) < 0 ||
        BCryptGetProperty(algorithm, BCRYPT_HASH_LENGTH,
                          reinterpret_cast<PUCHAR>(&hashLength), sizeof(hashLength), &copied, 0) < 0) {
        BCryptCloseAlgorithmProvider(algorithm, 0);
        return result;
    }
    std::vector<UCHAR> object(objectLength);
    std::vector<UCHAR> digest(hashLength);
    if (BCryptCreateHash(algorithm, &hash, object.data(), objectLength, nullptr, 0, 0) >= 0 &&
        BCryptHashData(hash, reinterpret_cast<PUCHAR>(const_cast<char*>(data.data())),
                       static_cast<ULONG>(data.size()), 0) >= 0 &&
        BCryptFinishHash(hash, digest.data(), hashLength, 0) >= 0) {
        std::ostringstream output;
        output << std::hex << std::setfill('0');
        for (UCHAR byte : digest) output << std::setw(2) << static_cast<unsigned>(byte);
        result = output.str();
    }
    if (hash != nullptr) BCryptDestroyHash(hash);
    BCryptCloseAlgorithmProvider(algorithm, 0);
    return result;
}

std::string frameBodyJson(const WorldStateFrame& frame) {
    const char* mode = gApp.sourceMode == SourceMode::Live ? "LIVE_NETWORK_COUNTERS" : "BUNDLED_REPLAY";
    const char* source = gApp.sourceMode == SourceMode::Live ? "windows-getiftable2" : "bundled-deterministic-load-v1";
    char frameId[48]{};
    sprintf_s(frameId, "%s-%09llu", gApp.sourceMode == SourceMode::Live ? "live" : "replay",
              static_cast<unsigned long long>(frame.sequence));
    std::ostringstream out;
    out << "{\"protocol\":\"world.state\",\"schemaVersion\":\"0.1.0\","
        << "\"sequence\":" << frame.sequence << ",\"frameId\":\"" << frameId << "\","
        << "\"producer\":{\"engine\":\"WorldEngine\",\"implementation\":\"cpp-win32\",\"version\":\"0.1.0\"},"
        << "\"provenance\":{\"mode\":\"" << mode << "\",\"sourceId\":\"" << source
        << "\",\"observedAtMs\":" << frame.observedAtMs << ",\"emittedAtMs\":" << frame.emittedAtMs << "},"
        << "\"observation\":{\"metric\":\"NETWORK_THROUGHPUT\",\"rawValue\":" << frame.rawValueBps
        << ",\"rawUnit\":\"bit_per_second\",\"rxBitPerSecond\":" << frame.rxBps
        << ",\"txBitPerSecond\":" << frame.txBps << ",\"normalizedValuePpm\":" << frame.normalizedValuePpm
        << ",\"normalizationReference\":" << frame.normalizationReferenceBps
        << ",\"sampleIntervalMs\":" << frame.sampleIntervalMs << ",\"ageMs\":" << frame.ageMs
        << ",\"quality\":{\"valid\":" << (frame.quality.valid ? "true" : "false")
        << ",\"counterReset\":" << (frame.quality.counterReset ? "true" : "false")
        << ",\"intervalStable\":" << (frame.quality.intervalStable ? "true" : "false")
        << ",\"capacityDeclared\":" << (frame.quality.capacityDeclared ? "true" : "false") << "}},"
        << "\"dynamics\":{\"changePpm\":" << frame.changePpm << ",\"variabilityPpm\":" << frame.variabilityPpm
        << ",\"noveltyPpm\":" << frame.noveltyPpm << "},"
        << "\"state\":{\"current\":\"" << stateNameAscii(frame.current) << "\",\"previous\":\""
        << stateNameAscii(frame.previous) << "\",\"dwellFrames\":" << frame.dwellFrames
        << ",\"reason\":\"" << frame.reason << "\"},"
        << "\"embodiment\":{\"intensityPpm\":" << frame.intensityPpm << ",\"energyPpm\":" << frame.energyPpm
        << ",\"urgencyPpm\":" << frame.urgencyPpm << "}";
    return out.str();
}

std::string completeFrameJson(WorldStateFrame& frame) {
    const std::string body = frameBodyJson(frame);
    frame.payloadHash = "sha256:" + sha256(body + "}");
    return body + ",\"integrity\":{\"canonicalization\":\"WORLD_JSON_V0_1\",\"payloadHash\":\"" +
           frame.payloadHash + "\"}}\n";
}

void initializeOutputPath() {
    wchar_t localAppData[MAX_PATH]{};
    if (SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, localAppData) != S_OK) {
        wcscpy_s(gApp.outputPath, L"world-state.json");
        return;
    }
    wchar_t directory[MAX_PATH]{};
    swprintf_s(directory, L"%s\\World2", localAppData);
    CreateDirectoryW(directory, nullptr);
    swprintf_s(gApp.outputPath, L"%s\\world-state.json", directory);
}

void writeFrameFile() {
    WorldStateFrame& frame = gApp.frame;
    frame.emittedAtMs = unixTimeMs();
    frame.ageMs = frame.emittedAtMs >= frame.observedAtMs
                      ? static_cast<std::uint32_t>(std::min<std::uint64_t>(UINT32_MAX, frame.emittedAtMs - frame.observedAtMs))
                      : 0;
    const std::string json = completeFrameJson(frame);
    std::wstring temporary = std::wstring(gApp.outputPath) + L".tmp";
    HANDLE file = CreateFileW(temporary.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    DWORD written = 0;
    WriteFile(file, json.data(), static_cast<DWORD>(json.size()), &written, nullptr);
    FlushFileBuffers(file);
    CloseHandle(file);
    MoveFileExW(temporary.c_str(), gApp.outputPath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
}

void pushHistory(std::int64_t value, bool valid) {
    gApp.history[static_cast<std::size_t>(gApp.historyNext)] = value;
    gApp.historyValid[static_cast<std::size_t>(gApp.historyNext)] = valid;
    gApp.historyNext = (gApp.historyNext + 1) % kHistoryCount;
    if (gApp.historyCount < kHistoryCount) ++gApp.historyCount;
}

std::int64_t recentMean(int limit, bool excludeNewest) {
    if (gApp.historyCount == 0) return 0;
    std::int64_t sum = 0;
    int count = 0;
    const int skip = excludeNewest ? 1 : 0;
    for (int offset = skip; offset < std::min(gApp.historyCount, limit + skip); ++offset) {
        const int index = (gApp.historyNext - 1 - offset + kHistoryCount) % kHistoryCount;
        if (!gApp.historyValid[static_cast<std::size_t>(index)]) continue;
        sum += gApp.history[static_cast<std::size_t>(index)];
        ++count;
    }
    return count == 0 ? 0 : sum / count;
}

std::int64_t recentVariability(int limit) {
    if (gApp.historyCount < 2) return 0;
    std::int64_t total = 0;
    int count = 0;
    for (int offset = 0; offset < std::min(gApp.historyCount - 1, limit); ++offset) {
        const int current = (gApp.historyNext - 1 - offset + kHistoryCount) % kHistoryCount;
        const int previous = (current - 1 + kHistoryCount) % kHistoryCount;
        if (!gApp.historyValid[static_cast<std::size_t>(current)] ||
            !gApp.historyValid[static_cast<std::size_t>(previous)]) continue;
        total += std::llabs(gApp.history[static_cast<std::size_t>(current)] -
                            gApp.history[static_cast<std::size_t>(previous)]);
        ++count;
    }
    return count == 0 ? 0 : clampPpm(total / count);
}

void deriveFrame(bool valid, bool counterReset, std::uint64_t bps, std::uint64_t rxBps,
                 std::uint64_t txBps, std::uint32_t intervalMs) {
    WorldStateFrame& frame = gApp.frame;
    const std::int64_t oldNormalized = frame.normalizedValuePpm;
    frame.sequence += 1;
    frame.observedAtMs = unixTimeMs();
    frame.rawValueBps = bps;
    frame.rxBps = rxBps;
    frame.txBps = txBps;
    frame.sampleIntervalMs = intervalMs;
    frame.normalizationReferenceBps = std::max<std::uint64_t>(UINT64_C(1000000), frame.normalizationReferenceBps);
    frame.normalizedValuePpm = valid
        ? clampPpm(static_cast<std::int64_t>((bps * static_cast<std::uint64_t>(kPpm)) /
                                             frame.normalizationReferenceBps))
        : 0;
    frame.quality = {valid, counterReset, intervalMs >= 100 && intervalMs <= 1000, true};
    frame.changePpm = valid ? clampPpm(std::llabs(frame.normalizedValuePpm - oldNormalized)) : 0;
    const std::int64_t baseline = recentMean(12, false);
    frame.noveltyPpm = valid ? clampPpm(std::llabs(frame.normalizedValuePpm - baseline)) : 0;
    pushHistory(frame.normalizedValuePpm, valid);
    frame.variabilityPpm = valid ? recentVariability(12) : 0;

    MotionState candidate = MotionState::Stale;
    const char* reason = valid ? "LOAD_LOW_PERSISTED" : "OBSERVATION_INVALID";
    if (valid && frame.normalizedValuePpm >= 550000) {
        candidate = MotionState::Storm;
        reason = "LOAD_HIGH_PERSISTED";
    } else if (valid && (frame.normalizedValuePpm >= 200000 || frame.noveltyPpm >= 120000 ||
                         frame.variabilityPpm >= 120000)) {
        candidate = MotionState::Attune;
        reason = "LOAD_CHANGE_DETECTED";
    } else if (valid) {
        candidate = MotionState::Calm;
    }

    if (candidate == MotionState::Stale) {
        gApp.pendingState = candidate;
        gApp.pendingFrames = 0;
    } else if (candidate == gApp.pendingState) {
        ++gApp.pendingFrames;
    } else {
        gApp.pendingState = candidate;
        gApp.pendingFrames = 1;
    }

    const bool switchNow = candidate == MotionState::Stale || frame.current == MotionState::Stale ||
                           candidate == frame.current || gApp.pendingFrames >= 3;
    if (switchNow && candidate != frame.current) {
        frame.previous = frame.current;
        frame.current = candidate;
        frame.dwellFrames = 1;
    } else {
        ++frame.dwellFrames;
    }
    frame.reason = reason;
    frame.intensityPpm = frame.normalizedValuePpm;
    frame.energyPpm = clampPpm(std::max(frame.normalizedValuePpm, frame.variabilityPpm));
    frame.urgencyPpm = clampPpm(std::max(frame.changePpm, frame.noveltyPpm));
    writeFrameFile();
}

void setInvalid(const wchar_t* message, bool counterReset) {
    wcsncpy_s(gApp.error, message, _TRUNCATE);
    deriveFrame(false, counterReset, 0, 0, 0, kSampleMs);
}

void sampleReplay() {
    gApp.error[0] = L'\0';
    const std::uint64_t phase = gApp.frame.sequence % 96;
    std::uint64_t bps = UINT64_C(900000);
    if (phase >= 24 && phase < 48) bps = UINT64_C(6500000);
    else if (phase >= 48 && phase < 72) bps = UINT64_C(17800000);
    else if (phase >= 72) bps = UINT64_C(2600000);
    deriveFrame(true, false, bps, (bps * 7) / 10, (bps * 3) / 10, kSampleMs);
}

void sampleNetwork() {
    if (gApp.sourceMode == SourceMode::Replay) {
        sampleReplay();
        return;
    }
    MIB_IF_TABLE2* table = nullptr;
    ULONGLONG rx = 0;
    ULONGLONG tx = 0;
    unsigned count = 0;
    LARGE_INTEGER now{};
    const ULONG result = GetIfTable2(&table);
    if (result != NO_ERROR || table == nullptr) {
        setInvalid(L"GetIfTable2 failed; no value was fabricated.", false);
        return;
    }
    for (ULONG index = 0; index < table->NumEntries; ++index) {
        const MIB_IF_ROW2& row = table->Table[index];
        if (row.OperStatus != IfOperStatusUp || row.Type == IF_TYPE_SOFTWARE_LOOPBACK ||
            row.MediaConnectState != MediaConnectStateConnected) continue;
        rx += row.InOctets;
        tx += row.OutOctets;
        ++count;
    }
    FreeMibTable(table);
    if (count == 0) {
        setInvalid(L"No connected, operational, non-loopback interface was found.", false);
        return;
    }
    QueryPerformanceCounter(&now);
    gApp.interfaceCount = count;
    gApp.error[0] = L'\0';
    if (!gApp.hasBaseline) {
        gApp.previousRx = rx;
        gApp.previousTx = tx;
        gApp.previousClock = now;
        gApp.hasBaseline = true;
        deriveFrame(true, false, 0, 0, 0, kSampleMs);
        return;
    }
    const LONGLONG ticks = now.QuadPart - gApp.previousClock.QuadPart;
    if (rx < gApp.previousRx || tx < gApp.previousTx || ticks <= 0) {
        gApp.hasBaseline = false;
        setInvalid(L"Counter or monotonic-clock discontinuity; baseline reset.", true);
        return;
    }
    const double seconds = static_cast<double>(ticks) /
                           static_cast<double>(gApp.clockFrequency.QuadPart);
    const ULONGLONG rxDelta = rx - gApp.previousRx;
    const ULONGLONG txDelta = tx - gApp.previousTx;
    const std::uint64_t rxBps = static_cast<std::uint64_t>(std::llround(rxDelta * 8.0 / seconds));
    const std::uint64_t txBps = static_cast<std::uint64_t>(std::llround(txDelta * 8.0 / seconds));
    gApp.previousRx = rx;
    gApp.previousTx = tx;
    gApp.previousClock = now;
    deriveFrame(true, false, rxBps + txBps, rxBps, txBps,
                static_cast<std::uint32_t>(std::llround(seconds * 1000.0)));
}

Bitmap* loadPngResource(int resourceId) {
    HMODULE module = GetModuleHandleW(nullptr);
    HRSRC resource = FindResourceW(module, MAKEINTRESOURCEW(resourceId), L"PNG");
    if (resource == nullptr) return nullptr;
    const DWORD size = SizeofResource(module, resource);
    HGLOBAL loaded = LoadResource(module, resource);
    const void* bytes = loaded == nullptr ? nullptr : LockResource(loaded);
    if (bytes == nullptr || size == 0) return nullptr;
    HGLOBAL copy = GlobalAlloc(GMEM_MOVEABLE, size);
    if (copy == nullptr) return nullptr;
    void* destination = GlobalLock(copy);
    memcpy(destination, bytes, size);
    GlobalUnlock(copy);
    IStream* stream = nullptr;
    if (CreateStreamOnHGlobal(copy, TRUE, &stream) != S_OK) {
        GlobalFree(copy);
        return nullptr;
    }
    Bitmap* streamed = Bitmap::FromStream(stream);
    Bitmap* bitmap = nullptr;
    if (streamed != nullptr && streamed->GetLastStatus() == Ok) {
        bitmap = streamed->Clone(0, 0, streamed->GetWidth(), streamed->GetHeight(), PixelFormat32bppARGB);
    }
    delete streamed;
    stream->Release();
    if (bitmap == nullptr || bitmap->GetLastStatus() != Ok) {
        delete bitmap;
        return nullptr;
    }
    return bitmap;
}

void addRoundedPath(GraphicsPath& path, const RectF& rect, REAL radius) {
    const REAL diameter = radius * 2.0F;
    path.AddArc(rect.X, rect.Y, diameter, diameter, 180.0F, 90.0F);
    path.AddArc(rect.GetRight() - diameter, rect.Y, diameter, diameter, 270.0F, 90.0F);
    path.AddArc(rect.GetRight() - diameter, rect.GetBottom() - diameter, diameter, diameter, 0.0F, 90.0F);
    path.AddArc(rect.X, rect.GetBottom() - diameter, diameter, diameter, 90.0F, 90.0F);
    path.CloseFigure();
}

void fillRounded(Graphics& graphics, const RectF& rect, REAL radius, Brush& brush) {
    GraphicsPath path;
    addRoundedPath(path, rect, radius);
    graphics.FillPath(&brush, &path);
}

void strokeRounded(Graphics& graphics, const RectF& rect, REAL radius, Pen& pen) {
    GraphicsPath path;
    addRoundedPath(path, rect, radius);
    graphics.DrawPath(&pen, &path);
}

void drawLabel(Graphics& graphics, const wchar_t* text, const RectF& rect, REAL size,
               INT style, const Color& color, StringAlignment alignment = StringAlignmentNear) {
    FontFamily family(L"Segoe UI");
    Font font(&family, size, style, UnitPixel);
    SolidBrush brush(color);
    StringFormat format;
    format.SetAlignment(alignment);
    format.SetLineAlignment(StringAlignmentCenter);
    format.SetTrimming(StringTrimmingEllipsisCharacter);
    graphics.DrawString(text, -1, &font, rect, &format, &brush);
}

void drawMetricCard(Graphics& graphics, const RectF& rect, const wchar_t* label,
                    const wchar_t* value, const Color& accent) {
    SolidBrush fill(Color(190, 18, 31, 50));
    fillRounded(graphics, rect, 14.0F, fill);
    Pen border(Color(55, accent.GetR(), accent.GetG(), accent.GetB()), 1.0F);
    strokeRounded(graphics, rect, 14.0F, border);
    drawLabel(graphics, label, RectF(rect.X + 13.0F, rect.Y + 7.0F, rect.Width - 26.0F, 20.0F),
              10.0F, FontStyleBold, Color(255, 133, 154, 181));
    drawLabel(graphics, value, RectF(rect.X + 13.0F, rect.Y + 26.0F, rect.Width - 26.0F, rect.Height - 31.0F),
              17.0F, FontStyleBold, Color(255, 235, 243, 252));
}

void drawEqualizer(Graphics& graphics, const RectF& rect, bool large) {
    Pen grid(Color(46, 118, 139, 163), 1.0F);
    for (int row = 0; row <= 4; ++row) {
        const REAL y = rect.Y + rect.Height * static_cast<REAL>(row) / 4.0F;
        graphics.DrawLine(&grid, rect.X, y, rect.GetRight(), y);
    }
    const int count = std::max(1, gApp.historyCount);
    const REAL gap = large ? 4.0F : 2.0F;
    const REAL barWidth = std::max(2.0F, (rect.Width - gap * (count - 1)) / count);
    for (int display = 0; display < count; ++display) {
        const int oldest = (gApp.historyNext - gApp.historyCount + kHistoryCount) % kHistoryCount;
        const int index = (oldest + display) % kHistoryCount;
        const bool valid = gApp.historyValid[static_cast<std::size_t>(index)];
        const std::int64_t ppm = gApp.history[static_cast<std::size_t>(index)];
        MotionState state = ppm < 200000 ? MotionState::Calm : ppm < 550000 ? MotionState::Attune : MotionState::Storm;
        const REAL height = valid ? 5.0F + static_cast<REAL>(ppm) / static_cast<REAL>(kPpm) * (rect.Height - 5.0F) : 2.0F;
        RectF bar(rect.X + display * (barWidth + gap), rect.GetBottom() - height, barWidth, height);
        SolidBrush brush(valid ? stateColor(state) : Color(80, 110, 124, 143));
        fillRounded(graphics, bar, std::min(3.0F, barWidth / 2.0F), brush);
    }
}

void drawDragon(Graphics& graphics, const RectF& rect) {
    Bitmap* bitmap = gApp.frame.current == MotionState::Storm ? gApp.dragonStorm
                    : gApp.frame.current == MotionState::Attune ? gApp.dragonAttune
                    : gApp.dragonCalm;
    if (bitmap == nullptr) {
        drawLabel(graphics, L"Embedded dragon asset unavailable", rect, 15.0F, FontStyleBold,
                  Color(255, 255, 154, 154), StringAlignmentCenter);
        return;
    }
    const REAL energy = static_cast<REAL>(gApp.frame.energyPpm) / static_cast<REAL>(kPpm);
    const REAL urgency = static_cast<REAL>(gApp.frame.urgencyPpm) / static_cast<REAL>(kPpm);
    // Breathing never stops. Network energy deepens and slightly quickens the respiratory cycle.
    const REAL breath = (static_cast<REAL>(std::sin(gApp.phase * (0.62 + energy * 0.38))) + 1.0F) * 0.5F;
    const REAL pulse = 1.0F + (breath - 0.5F) * (0.035F + energy * 0.055F);
    const REAL bob = static_cast<REAL>(std::sin(gApp.phase * 1.1)) * (3.0F + urgency * 13.0F);
    const REAL side = std::min(rect.Width, rect.Height) * 0.93F * pulse;
    const RectF destination(rect.X + (rect.Width - side) * 0.5F,
                            rect.Y + (rect.Height - side) * 0.5F + bob, side, side);
    ImageAttributes attributes;
    const REAL alpha = gApp.frame.current == MotionState::Stale ? 0.34F : 1.0F;
    ColorMatrix matrix = {{{1.0F, 0, 0, 0, 0}, {0, 1.0F, 0, 0, 0}, {0, 0, 1.0F, 0, 0},
                           {0, 0, 0, alpha, 0}, {0, 0, 0, 0, 1.0F}}};
    attributes.SetColorMatrix(&matrix);

    // Articulated wings sit behind the body. Each pivots at its shoulder; calm mode still has a
    // slow living flex, while measured network energy increases both flap rate and travel.
    const REAL flap = static_cast<REAL>(std::sin(gApp.phase * (1.45 + energy * 5.4)));
    const REAL travel = 9.0F + energy * 31.0F + urgency * 12.0F;
    const REAL shoulderY = destination.Y + destination.Height * 0.38F;
    const REAL shoulderOffset = destination.Width * 0.19F;
    const REAL wingLength = destination.Width * (0.34F + energy * 0.05F);
    const REAL wingDepth = destination.Height * (0.22F + breath * 0.035F);
    const Color accent = stateColor(gApp.frame.current);
    auto drawWing = [&](REAL shoulderX, REAL direction, REAL angle) {
        const GraphicsState saved = graphics.Save();
        graphics.TranslateTransform(shoulderX, shoulderY);
        graphics.RotateTransform(direction * angle);
        GraphicsPath wing;
        wing.StartFigure();
        wing.AddBezier(PointF(0.0F, 0.0F), PointF(direction * wingLength * 0.30F, -wingDepth * 0.58F),
                       PointF(direction * wingLength * 0.78F, -wingDepth * 0.74F),
                       PointF(direction * wingLength, -wingDepth * 0.20F));
        wing.AddBezier(PointF(direction * wingLength, -wingDepth * 0.20F),
                       PointF(direction * wingLength * 0.72F, -wingDepth * 0.02F),
                       PointF(direction * wingLength * 0.46F, wingDepth * 0.66F), PointF(0.0F, 0.0F));
        wing.CloseFigure();
        LinearGradientBrush membrane(RectF(-wingLength, -wingDepth, wingLength * 2.0F, wingDepth * 2.0F),
                                       Color(205, accent.GetR(), accent.GetG(), accent.GetB()),
                                       Color(72, 23, 34, 61), LinearGradientModeVertical);
        graphics.FillPath(&membrane, &wing);
        Pen bone(Color(225, 225, 238, 255), 2.2F + energy * 2.2F);
        graphics.DrawPath(&bone, &wing);
        graphics.DrawLine(&bone, 0.0F, 0.0F, direction * wingLength, -wingDepth * 0.20F);
        graphics.DrawLine(&bone, direction * wingLength * 0.48F, -wingDepth * 0.39F,
                          direction * wingLength * 0.47F, wingDepth * 0.37F);
        graphics.Restore(saved);
    };
    drawWing(destination.X + destination.Width * 0.5F - shoulderOffset, -1.0F, 8.0F + flap * travel);
    drawWing(destination.X + destination.Width * 0.5F + shoulderOffset, 1.0F, 8.0F + flap * travel);

    graphics.DrawImage(bitmap, destination, 0.0F, 0.0F, static_cast<REAL>(bitmap->GetWidth()),
                       static_cast<REAL>(bitmap->GetHeight()), UnitPixel, &attributes);

    // A chest pulse makes inhalation legible even when the embedded body art is visually dense.
    const REAL chestRadius = destination.Width * (0.055F + breath * 0.018F + energy * 0.012F);
    const PointF chest(destination.X + destination.Width * 0.50F,
                       destination.Y + destination.Height * 0.57F);
    SolidBrush chestGlow(Color(static_cast<BYTE>(28 + breath * 70 + energy * 55),
                               accent.GetR(), accent.GetG(), accent.GetB()));
    graphics.FillEllipse(&chestGlow, chest.X - chestRadius, chest.Y - chestRadius * 0.72F,
                         chestRadius * 2.0F, chestRadius * 1.44F);
    Pen breathRing(Color(static_cast<BYTE>(85 + breath * 120), 225, 244, 255), 1.2F + energy * 1.8F);
    graphics.DrawEllipse(&breathRing, chest.X - chestRadius, chest.Y - chestRadius * 0.72F,
                         chestRadius * 2.0F, chestRadius * 1.44F);
}

void drawField(Graphics& graphics, const RectF& rect) {
    const PointF center(rect.X + rect.Width * 0.5F, rect.Y + rect.Height * 0.52F);
    const Color accent = stateColor(gApp.frame.current);
    const REAL energy = static_cast<REAL>(gApp.frame.energyPpm) / static_cast<REAL>(kPpm);
    const REAL urgency = static_cast<REAL>(gApp.frame.urgencyPpm) / static_cast<REAL>(kPpm);
    for (int ring = 8; ring >= 1; --ring) {
        const REAL wave = static_cast<REAL>(std::sin(gApp.phase * (0.7 + urgency * 3.0) + ring * 0.74));
        const REAL radius = 24.0F + ring * 22.0F + wave * (4.0F + energy * 13.0F);
        Pen pen(Color(static_cast<BYTE>(22 + ring * 13), accent.GetR(), accent.GetG(), accent.GetB()),
                1.0F + energy * 3.0F);
        graphics.DrawEllipse(&pen, center.X - radius, center.Y - radius * 0.64F,
                             radius * 2.0F, radius * 1.28F);
    }
    SolidBrush core(Color(235, accent.GetR(), accent.GetG(), accent.GetB()));
    const REAL coreSize = 18.0F + energy * 35.0F;
    graphics.FillEllipse(&core, center.X - coreSize, center.Y - coreSize, coreSize * 2.0F, coreSize * 2.0F);
}

void drawSelector(Graphics& graphics, const RectF& rect, const wchar_t* label, bool selected,
                  const Color& accent) {
    SolidBrush fill(selected ? Color(68, accent.GetR(), accent.GetG(), accent.GetB()) : Color(150, 14, 25, 42));
    fillRounded(graphics, rect, 12.0F, fill);
    Pen border(selected ? Color(170, accent.GetR(), accent.GetG(), accent.GetB()) : Color(45, 130, 150, 178), 1.0F);
    strokeRounded(graphics, rect, 12.0F, border);
    drawLabel(graphics, label, rect, 11.0F, FontStyleBold,
              selected ? accent : Color(255, 149, 168, 192), StringAlignmentCenter);
}

void paintApp(HWND window, HDC target) {
    RECT client{};
    GetClientRect(window, &client);
    const INT width = std::max<LONG>(1, client.right);
    const INT height = std::max<LONG>(1, client.bottom);
    Bitmap buffer(width, height, PixelFormat32bppPARGB);
    Graphics graphics(&buffer);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
    LinearGradientBrush background(RectF(0, 0, static_cast<REAL>(width), static_cast<REAL>(height)),
                                   Color(255, 5, 12, 23), Color(255, 10, 8, 28), LinearGradientModeForwardDiagonal);
    graphics.FillRectangle(&background, 0, 0, width, height);
    const Color accent = stateColor(gApp.frame.current);
    SolidBrush glow(Color(20, accent.GetR(), accent.GetG(), accent.GetB()));
    graphics.FillEllipse(&glow, static_cast<REAL>(width) - 620.0F, -200.0F, 760.0F, 760.0F);

    const REAL margin = 34.0F;
    drawLabel(graphics, L"WORLD 2.0", RectF(margin, 20.0F, 230.0F, 34.0F), 26.0F, FontStyleBold,
              Color(255, 242, 247, 255));
    drawLabel(graphics, L"actual network counters → one frame → selectable embodiment",
              RectF(margin, 52.0F, 520.0F, 25.0F), 13.0F, FontStyleRegular, Color(255, 145, 166, 193));

    drawSelector(graphics, RectF(472.0F, 29.0F, 116.0F, 38.0F), L"DRAGON", gApp.pet == PetKind::Dragon, accent);
    drawSelector(graphics, RectF(598.0F, 29.0F, 116.0F, 38.0F), L"EQUALIZER", gApp.pet == PetKind::Equalizer, accent);
    drawSelector(graphics, RectF(724.0F, 29.0F, 96.0F, 38.0F), L"FIELD", gApp.pet == PetKind::Field, accent);
    const bool live = gApp.sourceMode == SourceMode::Live;
    drawSelector(graphics, RectF(static_cast<REAL>(width) - 243.0F, 29.0F, 209.0F, 38.0F),
                 live ? L"● LIVE COUNTERS" : L"▶ REPLAY (LABELLED)", true, accent);

    const REAL top = 96.0F;
    const REAL bottom = static_cast<REAL>(height) - 112.0F;
    const REAL leftWidth = static_cast<REAL>(width) * 0.58F;
    RectF dataCard(margin, top, leftWidth - margin - 10.0F, bottom - top);
    RectF petCard(leftWidth + 6.0F, top, static_cast<REAL>(width) - leftWidth - margin - 6.0F, bottom - top);
    LinearGradientBrush dataFill(dataCard, Color(223, 15, 28, 45), Color(220, 8, 17, 31), LinearGradientModeVertical);
    LinearGradientBrush petFill(petCard, Color(218, 17, 27, 50), Color(230, 7, 15, 29), LinearGradientModeForwardDiagonal);
    fillRounded(graphics, dataCard, 23.0F, dataFill);
    fillRounded(graphics, petCard, 23.0F, petFill);
    Pen border(Color(52, 142, 164, 194), 1.0F);
    strokeRounded(graphics, dataCard, 23.0F, border);
    Pen petBorder(Color(67, accent.GetR(), accent.GetG(), accent.GetB()), 1.0F);
    strokeRounded(graphics, petCard, 23.0F, petBorder);

    const double mbps = static_cast<double>(gApp.frame.rawValueBps) / 1000000.0;
    const double rxMbps = static_cast<double>(gApp.frame.rxBps) / 1000000.0;
    const double txMbps = static_cast<double>(gApp.frame.txBps) / 1000000.0;
    wchar_t totalText[64]{}, rxText[64]{}, txText[64]{}, loadText[64]{}, stateText[32]{};
    swprintf_s(totalText, L"%.2f Mbps", mbps);
    swprintf_s(rxText, L"%.2f Mbps", rxMbps);
    swprintf_s(txText, L"%.2f Mbps", txMbps);
    swprintf_s(loadText, L"%lld ppm", static_cast<long long>(gApp.frame.normalizedValuePpm));
    swprintf_s(stateText, L"%s", stateName(gApp.frame.current));
    drawLabel(graphics, L"USEFUL NETWORK MONITOR", RectF(dataCard.X + 22.0F, dataCard.Y + 13.0F, 300.0F, 25.0F),
              11.0F, FontStyleBold, Color(255, 132, 154, 181));
    const REAL metricY = dataCard.Y + 48.0F;
    const REAL metricGap = 9.0F;
    const REAL metricWidth = (dataCard.Width - 44.0F - metricGap * 2.0F) / 3.0F;
    drawMetricCard(graphics, RectF(dataCard.X + 22.0F, metricY, metricWidth, 76.0F), L"TOTAL", totalText, accent);
    drawMetricCard(graphics, RectF(dataCard.X + 22.0F + metricWidth + metricGap, metricY, metricWidth, 76.0F), L"DOWNLOAD", rxText, accent);
    drawMetricCard(graphics, RectF(dataCard.X + 22.0F + (metricWidth + metricGap) * 2.0F, metricY, metricWidth, 76.0F), L"UPLOAD", txText, accent);
    drawMetricCard(graphics, RectF(dataCard.X + 22.0F, metricY + 87.0F, metricWidth, 70.0F), L"NORMALIZED LOAD", loadText, accent);
    drawMetricCard(graphics, RectF(dataCard.X + 22.0F + metricWidth + metricGap, metricY + 87.0F, metricWidth, 70.0F), L"WORLD STATE", stateText, accent);
    wchar_t referenceText[48]{};
    swprintf_s(referenceText, L"%.1f Mbps", static_cast<double>(gApp.frame.normalizationReferenceBps) / 1000000.0);
    drawMetricCard(graphics, RectF(dataCard.X + 22.0F + (metricWidth + metricGap) * 2.0F, metricY + 87.0F, metricWidth, 70.0F),
                   L"REFERENCE", referenceText, accent);
    RectF history(dataCard.X + 22.0F, dataCard.Y + 226.0F, dataCard.Width - 44.0F, dataCard.Height - 294.0F);
    drawEqualizer(graphics, history, false);
    drawLabel(graphics, L"64 samples · left → right in time · not RF frequency bands",
              RectF(history.X, history.GetBottom() + 7.0F, history.Width, 23.0F), 11.0F, FontStyleRegular,
              Color(255, 124, 147, 176));

    drawLabel(graphics, petName(gApp.pet), RectF(petCard.X + 22.0F, petCard.Y + 13.0F, petCard.Width - 44.0F, 25.0F),
              11.0F, FontStyleBold, Color(255, 132, 154, 181));
    drawLabel(graphics, stateName(gApp.frame.current), RectF(petCard.X + 22.0F, petCard.Y + 37.0F, petCard.Width - 44.0F, 37.0F),
              25.0F, FontStyleBold, accent);
    RectF embodiment(petCard.X + 16.0F, petCard.Y + 73.0F, petCard.Width - 32.0F, petCard.Height - 115.0F);
    if (gApp.pet == PetKind::Dragon) drawDragon(graphics, embodiment);
    else if (gApp.pet == PetKind::Equalizer) drawEqualizer(graphics, embodiment, true);
    else drawField(graphics, embodiment);
    wchar_t frameText[200]{};
    swprintf_s(frameText, L"intensity %lld · energy %lld · urgency %lld ppm",
               static_cast<long long>(gApp.frame.intensityPpm), static_cast<long long>(gApp.frame.energyPpm),
               static_cast<long long>(gApp.frame.urgencyPpm));
    drawLabel(graphics, frameText, RectF(petCard.X + 20.0F, petCard.GetBottom() - 39.0F, petCard.Width - 40.0F, 25.0F),
              11.0F, FontStyleRegular, Color(255, 183, 203, 226), StringAlignmentCenter);

    RectF footer(margin, static_cast<REAL>(height) - 94.0F, static_cast<REAL>(width) - margin * 2.0F, 66.0F);
    SolidBrush footerFill(Color(184, 10, 19, 33));
    fillRounded(graphics, footer, 17.0F, footerFill);
    drawLabel(graphics, live ? L"MEASURED LIVE" : L"BUNDLED REPLAY", RectF(footer.X + 18.0F, footer.Y + 8.0F, 145.0F, 24.0F),
              10.0F, FontStyleBold, accent);
    drawLabel(graphics, live ? L"Windows GetIfTable2 · raw counter deltas / measured interval"
                             : L"Deterministic fallback · never presented as a live measurement",
              RectF(footer.X + 163.0F, footer.Y + 5.0F, footer.Width - 181.0F, 29.0F), 12.0F, FontStyleRegular,
              Color(255, 207, 221, 237));
    drawLabel(graphics, L"M mode · D/E/F Pet · ↑/↓ capacity · R reset · T pin · frame exported to LocalAppData\\World2",
              RectF(footer.X + 18.0F, footer.Y + 34.0F, footer.Width - 36.0F, 23.0F), 11.0F, FontStyleRegular,
              Color(255, 128, 150, 178));
    if (!gApp.frame.quality.valid) {
        RectF errorRect(margin + 280.0F, 30.0F, 450.0F, 38.0F);
        SolidBrush errorFill(Color(230, 53, 18, 41));
        fillRounded(graphics, errorRect, 11.0F, errorFill);
        drawLabel(graphics, gApp.error, errorRect, 11.0F, FontStyleBold, Color(255, 255, 170, 185), StringAlignmentCenter);
    }
    Graphics targetGraphics(target);
    targetGraphics.DrawImage(&buffer, 0, 0);
}

void resetSampling() {
    gApp.hasBaseline = false;
    gApp.historyCount = 0;
    gApp.historyNext = 0;
    gApp.pendingFrames = 0;
    gApp.error[0] = L'\0';
    sampleNetwork();
}

void toggleMode() {
    gApp.sourceMode = gApp.sourceMode == SourceMode::Live ? SourceMode::Replay : SourceMode::Live;
    resetSampling();
}

LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE:
            SetTimer(window, kTimerId, kSampleMs, nullptr);
            sampleNetwork();
            return 0;
        case WM_TIMER:
            if (wparam == kTimerId) {
                gApp.phase += 0.16 + static_cast<double>(gApp.frame.energyPpm) / kPpm * 0.32;
                sampleNetwork();
                InvalidateRect(window, nullptr, FALSE);
            }
            return 0;
        case WM_LBUTTONUP: {
            const int x = GET_X_LPARAM(lparam);
            const int y = GET_Y_LPARAM(lparam);
            RECT client{};
            GetClientRect(window, &client);
            if (y >= 29 && y <= 67) {
                if (x >= 472 && x <= 588) gApp.pet = PetKind::Dragon;
                else if (x >= 598 && x <= 714) gApp.pet = PetKind::Equalizer;
                else if (x >= 724 && x <= 820) gApp.pet = PetKind::Field;
                else if (x >= client.right - 243 && x <= client.right - 34) toggleMode();
            }
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        }
        case WM_KEYDOWN:
            if (wparam == VK_UP) {
                gApp.frame.normalizationReferenceBps = std::min<std::uint64_t>(UINT64_C(100000000000),
                    (gApp.frame.normalizationReferenceBps * 5) / 4);
            } else if (wparam == VK_DOWN) {
                gApp.frame.normalizationReferenceBps = std::max<std::uint64_t>(UINT64_C(1000000),
                    (gApp.frame.normalizationReferenceBps * 4) / 5);
            } else if (wparam == 'R') resetSampling();
            else if (wparam == 'M') toggleMode();
            else if (wparam == 'D') gApp.pet = PetKind::Dragon;
            else if (wparam == 'E') gApp.pet = PetKind::Equalizer;
            else if (wparam == 'F') gApp.pet = PetKind::Field;
            else if (wparam == 'T') {
                gApp.alwaysOnTop = !gApp.alwaysOnTop;
                SetWindowPos(window, gApp.alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0,
                             SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            }
            InvalidateRect(window, nullptr, FALSE);
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case WM_PAINT: {
            PAINTSTRUCT paint{};
            HDC context = BeginPaint(window, &paint);
            paintApp(window, context);
            EndPaint(window, &paint);
            return 0;
        }
        case WM_DESTROY:
            KillTimer(window, kTimerId);
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(window, message, wparam, lparam);
    }
}

double commandCapacity(const wchar_t* commandLine) {
    const wchar_t* option = wcsstr(commandLine, L"--capacity-mbps=");
    if (option == nullptr) return 20.0;
    option += wcslen(L"--capacity-mbps=");
    const double value = _wtof(option);
    return value >= 1.0 && value <= 100000.0 ? value : 20.0;
}

}  // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previous, PWSTR commandLine, int showCommand) {
    (void)previous;
    SetProcessDPIAware();
    QueryPerformanceFrequency(&gApp.clockFrequency);
    gApp.frame.normalizationReferenceBps =
        static_cast<std::uint64_t>(std::llround(commandCapacity(commandLine) * 1000000.0));
    initializeOutputPath();
    GdiplusStartupInput startupInput;
    if (GdiplusStartup(&gGdiplusToken, &startupInput, nullptr) != Ok) return 1;
    gApp.dragonCalm = loadPngResource(IDR_DRAGON_CALM);
    gApp.dragonAttune = loadPngResource(IDR_DRAGON_ATTUNE);
    gApp.dragonStorm = loadPngResource(IDR_DRAGON_STORM);
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = windowProc;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    windowClass.lpszClassName = L"World2LivePetsWindow";
    if (RegisterClassExW(&windowClass) == 0) return 2;
    HWND window = CreateWindowExW(0, windowClass.lpszClassName, kAppTitle,
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                                  1280, 780, nullptr, nullptr, instance, nullptr);
    if (window == nullptr) return 3;
    ShowWindow(window, showCommand);
    UpdateWindow(window);
    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    delete gApp.dragonCalm;
    delete gApp.dragonAttune;
    delete gApp.dragonStorm;
    GdiplusShutdown(gGdiplusToken);
    return static_cast<int>(message.wParam);
}
