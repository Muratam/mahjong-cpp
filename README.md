# mahjong-cpp

## About

fork of https://github.com/nekobean/mahjong-cpp .

emscripten compiled version.

## How to build and run

```bash
# build
mkdir build && cd build
emcmake cmake ..
emmake make
# copy files to server
cp ./src/emscripten/main.* /path/to/dir
# in brwoser
> load: <script src="main.js"></script>
> Module.run_sample()
> Module.process_request('{"zikaze":27,"bakaze":27,"turn":3,"syanten_type":1,"dora_indicators":[27],"flag":63,"hand_tiles":[0,34,6,9,11,12,13,35,13,17,20,23,24,25],"melded_blocks":[]}')
```

## Usage

* [向聴数計算 (Syanten Number Calculation)](src/samples/sample_calculate_syanten.cpp)
* [点数計算 (Score Calculation)](src/samples/sample_calculate_score.cpp)
* [有効牌選択 (Required Tile Selection)](src/samples/sample_required_tile_selector.cpp)
* [不要牌選択 (Unnecessary Tile Selection)](src/samples/sample_unnecessary_tile_selector.cpp)
* [期待値計算 (Expected Value Calculation)](src/samples/sample_calculate_expexted_value.cpp)
