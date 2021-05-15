#include <fstream>

#include "mahjong/handseparator.hpp"
#include "mahjong/json_parser.hpp"
#include "mahjong/mahjong.hpp"
#include "mahjong/syanten.hpp"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/schema.h"

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace mahjong;
namespace
{
std::tuple<bool, RequestData> parse_json(const rapidjson::Document &doc)
{
    RequestData req = parse_request(doc);

    std::string dora_indicators = "";
    for (const auto &tile : req.dora_indicators) {
        dora_indicators += Tile::Name.at(tile) + (&tile != &req.dora_indicators.back() ? " " : "");
    }
    auto counts = ExpectedValueCalculator::count_left_tiles(req.hand, req.dora_indicators);
    for (auto x : counts) {
        if (x < 0) {
            return {false, req};
        }
    }
    return {true, req};
}

} // namespace

std::string process_request(const std::string &json)
{
    rapidjson::Document doc(rapidjson::kObjectType);
    // リクエストデータを読み込む。
    rapidjson::Document req_doc;
    req_doc.Parse(json.c_str());
    if (req_doc.HasParseError()) {
        doc.AddMember("success", false, doc.GetAllocator());
        doc.AddMember("err_msg", "Failed to parse request data (invalid json format).",
                      doc.GetAllocator());
        return to_json_str(doc);
    }

    std::ifstream ifs("res/request_schema.json");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document sd;
    sd.ParseStream(isw);
    rapidjson::SchemaDocument schema(sd);
    rapidjson::SchemaValidator validator(schema);

    if (!req_doc.Accept(validator)) {
        doc.AddMember("success", false, doc.GetAllocator());
        doc.AddMember("request", req_doc.GetObject(), doc.GetAllocator());
        doc.AddMember("err_msg", "Failed to parse request data (invalid value found).",
                      doc.GetAllocator());
        return to_json_str(doc);
    }

    // JSON を解析する。
    auto [success, req] = parse_json(req_doc);
    if (!success) {
        doc.AddMember("success", false, doc.GetAllocator());
        doc.AddMember("request", req_doc.GetObject(), doc.GetAllocator());
        doc.AddMember("err_msg", "Failed to parse request data (invalid hand found).",
                      doc.GetAllocator());
        return to_json_str(doc);
    }

    // 向聴数を計算する。
    auto [syanten_type, syanten] = SyantenCalculator::calc(req.hand, req.syanten_type);
    if (syanten == -1) {
        doc.AddMember("success", false, doc.GetAllocator());
        doc.AddMember("request", req_doc.GetObject(), doc.GetAllocator());
        doc.AddMember("err_msg", "和了形です。", doc.GetAllocator());
        return to_json_str(doc);
    }

    // 計算する。
    auto res_doc = create_response(req, doc);
    // 出力用 JSON を作成する。
    doc.AddMember("success", true, doc.GetAllocator());
    doc.AddMember("request", req_doc.GetObject(), doc.GetAllocator());
    doc.AddMember("response", res_doc, doc.GetAllocator());
    return to_json_str(doc);
}

void run_sample()
{
    ScoreCalculator score;
    // 場やルールの設定
    score.set_bakaze(Tile::Ton);             // 場風牌
    score.set_zikaze(Tile::Ton);             // 自風牌
    score.set_num_tumibo(0);                 // 積み棒の数
    score.set_num_kyotakubo(0);              // 供託棒の数
    score.set_dora_tiles({Tile::Pe});        // ドラの一覧 (表示牌ではない)
    score.set_uradora_tiles({Tile::Pinzu9}); // 裏ドラの一覧 (表示牌ではない)

    // 手牌、和了牌、フラグの設定
    // 手牌
    MeldedBlock block(MeldType::Kakan, {Tile::Ton, Tile::Ton, Tile::Ton, Tile::Ton});
    Hand hand({Tile::Manzu1, Tile::Manzu2, Tile::Manzu3, Tile::Pinzu3, Tile::Pinzu4, Tile::Pinzu5,
               Tile::Sozu1, Tile::Sozu2, Tile::Sozu3, Tile::Sozu4, Tile::Sozu4},
              {block});
    int win_tile = Tile::Manzu1;                        // 和了牌
    int flag = HandFlag::Tumo | HandFlag::Rinsyankaiho; // フラグ

    // 点数計算
    Result ret = score.calc(hand, win_tile, flag);
    std::cout << ret.to_string() << std::endl;
}

EMSCRIPTEN_BINDINGS(mahjong)
{
    emscripten::function("process_request", process_request);
    emscripten::function("run_sample", run_sample);
}

int main()
{
    SyantenCalculator::initialize();
    HandSeparator::initialize();
}
