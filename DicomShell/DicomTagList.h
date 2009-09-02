#pragma once

#include <vector>
#include <string>

class DicomTagList
{
public:
	DicomTagList(LPCTSTR key);
	~DicomTagList(void);
	typedef std::vector<DcmTagKey> TagCollection;
	TagCollection const& getTags();
private:
	std::vector<DcmTagKey> m_tags;
	bool m_valid;
	std::wstring m_dicPath;
	FILETIME m_dicTime;
	std::wstring m_key;
};
