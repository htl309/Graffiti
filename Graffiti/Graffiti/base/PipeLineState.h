#pragma once

namespace Graffiti {



    enum class PipelineState : uint32_t
    {
        Common = 0,
        WireFrame = GF_BIT(0),	//0000 0001
        DepthTestDisable = GF_BIT(1),			//0000 0010
        MeshShaderPipeLine= GF_BIT(2),
        // 可继续扩展更多状态...
    };
    // 按位或
    inline PipelineState operator|(PipelineState a, PipelineState b)
    {
        return static_cast<PipelineState>(
            static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    // 按位与
    inline PipelineState operator&(PipelineState a, PipelineState b)
    {
        return static_cast<PipelineState>(
            static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    // 按位取反
    inline PipelineState operator~(PipelineState a)
    {
        return static_cast<PipelineState>(
            ~static_cast<uint32_t>(a));
    }

    // 异或（可选）
    inline PipelineState operator^(PipelineState a, PipelineState b)
    {
        return static_cast<PipelineState>(
            static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
    }

    // 复合运算符： |=
    inline PipelineState& operator|=(PipelineState& a, PipelineState b)
    {
        a = a | b;
        return a;
    }

    // 复合运算符： &=
    inline PipelineState& operator&=(PipelineState& a, PipelineState b)
    {
        a = a & b;
        return a;
    }

    // 复合运算符： ^=
    inline PipelineState& operator^=(PipelineState& a, PipelineState b)
    {
        a = a ^ b;
        return a;
    }

    inline bool HasState(PipelineState value, PipelineState test)
    {
        return (value & test) != PipelineState::Common;
    }

}