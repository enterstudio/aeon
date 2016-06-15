#pragma once

#include "provider.hpp"
#include "etl_label.hpp"
#include "etl_image.hpp"

namespace nervana {
    namespace image {
        class randomizing_provider;
    }
    namespace label {
        class binary_provider;
    }

    class image::randomizing_provider : public provider<image::decoded, image::params> {
    public:
        randomizing_provider(string& cfg_str, int seed=0)
        {
            _cfg         = std::make_shared<image::config>(cfg_str);
            _r_eng       = std::default_random_engine(seed);
            _extractor   = std::make_shared<image::extractor>(_cfg);
            _transformer = std::make_shared<image::transformer>(_cfg);
            _loader      = std::make_shared<image::loader>(_cfg);
            _factory     = std::make_shared<image::param_factory>(_cfg, _r_eng);
        }
        ~randomizing_provider() {}

    private:
        std::default_random_engine     _r_eng;
        std::shared_ptr<image::config> _cfg;
    };

    class label::binary_provider : public provider<label::decoded, nervana::params> {
    public:
        binary_provider()
        {
            _extractor   = std::make_shared<label::extractor>();
            _transformer = std::make_shared<label::transformer>();
            _loader      = std::make_shared<label::loader>();
            _factory     = nullptr;
        }
        ~binary_provider() {}
    };

    class image_decoder : public train_provider<image::randomizing_provider, label::binary_provider> {
    public:
        image_decoder(string& img_cfg_str, int img_out_sz, int tgt_out_sz, int seed=0)
        {
            _dprov = std::make_shared<image::randomizing_provider>(img_cfg_str, seed);
            _tprov = std::make_shared<label::binary_provider>();
            _dsz_out = img_out_sz;
            _tsz_out = tgt_out_sz;
        }
    };
}