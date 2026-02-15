#pragma once

#include "chess_data.hpp"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>

// Not mine, its from godot | I needed to bind my Enum class
#define VARIANT_ENUM_CLASS_CAST(m_enum)                                 \
namespace godot {                                                       \
    template <>                                                         \
    struct VariantCaster<m_enum> {                                      \
        static _FORCE_INLINE_ m_enum cast(const Variant &p_variant) {   \
            return (m_enum)p_variant.operator int64_t();                \
        }                                                               \
    };                                                                  \
    template <>                                                         \
    struct PtrToArg<m_enum> {                                           \
        using EnumClass = std::underlying_type_t<m_enum>;               \
        _FORCE_INLINE_ static m_enum convert(const void *p_ptr) {       \
            return m_enum(*reinterpret_cast<const EnumClass *>(p_ptr)); \
        }                                                               \
        typedef int64_t EncodeT;                                        \
        _FORCE_INLINE_ static void encode(m_enum p_val, void *p_ptr) {  \
            *reinterpret_cast<EnumClass *>(p_ptr) = (EnumClass)p_val;   \
        }                                                               \
    };                                                                  \
}

class ChessPieces : public godot::Resource {
    GDCLASS(ChessPieces, godot::Resource)
private:
    int enum_holder{};
    void set_enum(int x) {enum_holder = x;}
    int get_enum() const {return enum_holder;}

    static void _bind_methods() {
        constexpr int PAWN   = int(Pieces::Pawn);   BIND_ENUM_CONSTANT(PAWN);
        constexpr int ROOK   = int(Pieces::Rook);   BIND_ENUM_CONSTANT(ROOK);
        constexpr int KNIGHT = int(Pieces::Knight); BIND_ENUM_CONSTANT(KNIGHT);
        constexpr int BISHOP = int(Pieces::Bishop); BIND_ENUM_CONSTANT(BISHOP);
        constexpr int QUEEN  = int(Pieces::Queen);  BIND_ENUM_CONSTANT(QUEEN);
        constexpr int KING   = int(Pieces::King);   BIND_ENUM_CONSTANT(KING);

        godot::ClassDB::bind_method(godot::D_METHOD("set_enum", "x"), &ChessPieces::set_enum);
        godot::ClassDB::bind_method(godot::D_METHOD("get_enum"), &ChessPieces::get_enum);
            godot::ClassDB::add_property(
            get_class_static(),
            godot::PropertyInfo(
                godot::Variant::INT,
                "enum_holder",
                godot::PROPERTY_HINT_ENUM,
                "Pawn, Rook, Knight, Bishop, Queen, King"
            ),"set_enum","get_enum"
        );
    }
};

VARIANT_ENUM_CLASS_CAST(Pieces)
