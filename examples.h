#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <QList>
#include <QHash>

class Example
{
public:
    Example() : Example("", "", "") { }
    Example(const QString &name, const QString &text, const QString &url)
        : m_name(name), m_logo(""), m_yaml(""), m_text(text), m_url(url)
    {
    }

    QString name() const { return m_name; }
    QString logo() const { return m_logo.isEmpty() ? m_name + ".png" : m_logo; }
    void setLogo(QString logo) { m_logo = logo; }
    QString yaml() const { return m_yaml.isEmpty() ? m_name + ".yaml" : m_yaml; }
    void setYaml(QString yaml) { m_yaml = yaml; }
    QString text() const { return m_text; }
    void setText(QString text) { m_text = text; }
    QString url() const { return m_url; }
    void setUrl(QString url) { m_url = url; }

private:
    QString m_name;
    QString m_logo;
    QString m_yaml;
    QString m_text;
    QString m_url;
};

typedef QList<Example> ExampleList;
typedef QHash<QString, Example> ExampleHash;

extern ExampleHash &getExamples();

#endif // EXAMPLES_H
