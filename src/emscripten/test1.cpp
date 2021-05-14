#include "mahjong/handseparator.hpp"
#include "mahjong/mahjong.hpp"
#include "mahjong/syanten.hpp"
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

using namespace mahjong;
int main()
{
#ifdef EMSCRIPTEN
    EM_ASM(FS.mkdir('res'); FS.mount(NODEFS, {root : '.'}, 'res'););
#endif
    SyantenCalculator::initialize();
    HandSeparator::initialize();
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
