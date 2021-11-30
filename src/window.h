//
// Created by dan on 19/11/2021.
//

#ifndef FRACTALS_WINDOW_H
#define FRACTALS_WINDOW_H

#include "common.h"
#include "stx/internal/option_result.h"
#include <stx/result.h>

#include "gfx/vertexarray.h"
#include "gfx/shaderprogram.h"

namespace fr {
    namespace window {
        enum class Error {
            InvalidPath
        };

        struct WindowSettings {
            int width;
            int height;
            bool fullscreen;
        };

        /**
         * Retrieve window settings from the config file. WindowSettings is used
         * to construct the Window class
         * @param path relative path to config file
         * @return Result type wrapping the struct
         */
        auto get_window_settings(const std::string &path) -> stx::Result<WindowSettings, Error>;

        class Window {
        public:
            Window(SDL_Window &wnd, const WindowSettings &w) noexcept;
            ~Window() = default;

            auto init() -> void;
            auto update(float ms) -> void;
            auto gui() -> void;

            auto on_resize(int w, int h) -> void;
            auto on_keydown(const SDL_KeyboardEvent& key_ev) -> void;

        private:
            SDL_Window *wnd_;
            int width_;
            int height_;
            bool fullscreen_;
            float runtime_;

            // rendering objects
            std::shared_ptr<VertexBuffer> vbo_;
            std::shared_ptr<ShaderProgram> shader_;
            std::shared_ptr<VertexArray> va_;

            // shader properties
            float zoom_;
            int iterations_ = 900;
            float offsetX_ = 0.0f;
            float offsetY_ = 0.0f;
        };
    }
}

#endif //FRACTALS_WINDOW_H
