#pragma once

#include <optional>
#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "common/helpers/print_helpers.h"


namespace dto
{

/**
 * @brief CommentDocument DTO
 */
class CommentDocument final
{
public:
    using Ptr = std::shared_ptr<CommentDocument>;
    using ConstPtr = std::shared_ptr<const CommentDocument>;

public:
    CommentDocument() = default;
    explicit CommentDocument(const nlohmann::json& json);

    // Поле: «id» Уникальный идентификатор
    const std::optional<int64_t>& id() const { return m_id; }
    bool hasId() const { return m_id.has_value(); }
    void setId(const int64_t& value)
    {
        m_id = value;
    }
    void setId(int64_t&& value)
    {
        m_id = std::move(value);
    }
    void clearId()
    {
        m_id = std::nullopt;
    }

    // Поле: «commentId» Идентификатор комментария
    const std::optional<int64_t>& commentId() const { return m_commentId; }
    bool hasCommentId() const { return m_commentId.has_value(); }
    void setCommentId(const int64_t& value)
    {
        m_commentId = value;
    }
    void setCommentId(int64_t&& value)
    {
        m_commentId = std::move(value);
    }
    void clearCommentId()
    {
        m_commentId = std::nullopt;
    }

    // Поле: «documentId» Идентификатор документа
    const std::optional<int64_t>& documentId() const { return m_documentId; }
    bool hasDocumentId() const { return m_documentId.has_value(); }
    void setDocumentId(const int64_t& value)
    {
        m_documentId = value;
    }
    void setDocumentId(int64_t&& value)
    {
        m_documentId = std::move(value);
    }
    void clearDocumentId()
    {
        m_documentId = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const CommentDocument& other) const;
    bool operator!=(const CommentDocument& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const CommentDocument& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<int64_t> m_commentId;
    std::optional<int64_t> m_documentId;
};

inline std::ostream& operator<<(std::ostream& os, const CommentDocument& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto