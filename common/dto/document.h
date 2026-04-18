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
 * @brief Document DTO
 */
class Document final
{
public:
    using Ptr = std::shared_ptr<Document>;
    using ConstPtr = std::shared_ptr<const Document>;

public:
    Document() = default;
    explicit Document(const nlohmann::json& json);

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

    // Поле: «caption» Название документа
    const std::optional<std::string>& caption() const { return m_caption; }
    bool hasCaption() const { return m_caption.has_value(); }
    void setCaption(const std::string& value)
    {
        m_caption = value;
    }
    void setCaption(std::string&& value)
    {
        m_caption = std::move(value);
    }
    void clearCaption()
    {
        m_caption = std::nullopt;
    }

    // Поле: «description» Описание документа
    const std::optional<std::string>& description() const { return m_description; }
    bool hasDescription() const { return m_description.has_value(); }
    void setDescription(const std::string& value)
    {
        m_description = value;
    }
    void setDescription(std::string&& value)
    {
        m_description = std::move(value);
    }
    void clearDescription()
    {
        m_description = std::nullopt;
    }

    // Поле: «path» Путь к файлу на диске
    const std::optional<std::string>& path() const { return m_path; }
    bool hasPath() const { return m_path.has_value(); }
    void setPath(const std::string& value)
    {
        m_path = value;
    }
    void setPath(std::string&& value)
    {
        m_path = std::move(value);
    }
    void clearPath()
    {
        m_path = std::nullopt;
    }

    // Поле: «filename» Исходное имя файла
    const std::optional<std::string>& filename() const { return m_filename; }
    bool hasFilename() const { return m_filename.has_value(); }
    void setFilename(const std::string& value)
    {
        m_filename = value;
    }
    void setFilename(std::string&& value)
    {
        m_filename = std::move(value);
    }
    void clearFilename()
    {
        m_filename = std::nullopt;
    }

    // Поле: «size» Размер файла в байтах
    const std::optional<int64_t>& size() const { return m_size; }
    bool hasSize() const { return m_size.has_value(); }
    void setSize(const int64_t& value)
    {
        m_size = value;
    }
    void setSize(int64_t&& value)
    {
        m_size = std::move(value);
    }
    void clearSize()
    {
        m_size = std::nullopt;
    }

    // Поле: «mimeType» MIME-тип файла
    const std::optional<std::string>& mimeType() const { return m_mimeType; }
    bool hasMimeType() const { return m_mimeType.has_value(); }
    void setMimeType(const std::string& value)
    {
        m_mimeType = value;
    }
    void setMimeType(std::string&& value)
    {
        m_mimeType = std::move(value);
    }
    void clearMimeType()
    {
        m_mimeType = std::nullopt;
    }

    // Поле: «uploadedAt» Время загрузки
    const std::optional<std::chrono::system_clock::time_point>& uploadedAt() const { return m_uploadedAt; }
    bool hasUploadedAt() const { return m_uploadedAt.has_value(); }
    void setUploadedAt(const std::chrono::system_clock::time_point& value)
    {
        m_uploadedAt = value;
    }
    void setUploadedAt(std::chrono::system_clock::time_point&& value)
    {
        m_uploadedAt = std::move(value);
    }
    void clearUploadedAt()
    {
        m_uploadedAt = std::nullopt;
    }

    // Поле: «uploadedByUserId» Идентификатор пользователя
    const std::optional<int64_t>& uploadedByUserId() const { return m_uploadedByUserId; }
    bool hasUploadedByUserId() const { return m_uploadedByUserId.has_value(); }
    void setUploadedByUserId(const int64_t& value)
    {
        m_uploadedByUserId = value;
    }
    void setUploadedByUserId(int64_t&& value)
    {
        m_uploadedByUserId = std::move(value);
    }
    void clearUploadedByUserId()
    {
        m_uploadedByUserId = std::nullopt;
    }

    // Сериализация
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json& json);

    // Валидация
    bool isValid() const;
    std::string validationError() const;

    // Сравнение
    bool operator==(const Document& other) const;
    bool operator!=(const Document& other) const
    {
        return !(*this == other);
    }

    // Потоковый вывод для отладки
    friend std::ostream& operator<<(std::ostream& os, const Document& dto);

private:
    std::optional<int64_t> m_id;
    std::optional<std::string> m_caption;
    std::optional<std::string> m_description;
    std::optional<std::string> m_path;
    std::optional<std::string> m_filename;
    std::optional<int64_t> m_size;
    std::optional<std::string> m_mimeType;
    std::optional<std::chrono::system_clock::time_point> m_uploadedAt;
    std::optional<int64_t> m_uploadedByUserId;
};

inline std::ostream& operator<<(std::ostream& os, const Document& dto)
{
    os << dto.toJson().dump(2);
    return os;
}

} // namespace dto