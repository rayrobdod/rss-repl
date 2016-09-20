
#include "FeedElement.h"
#include <hubbub/parser.h>

using std::pair;
using std::wstring;

bool isWhitespace(const wchar_t c) {
	return c == L' ' || c == L'\t' || c == L'\n';
}

std::string hubbubstr2stdstr(const hubbub_string in) {
	return std::string((char*) in.ptr, in.len);
}

// Because hubbub doesn't do whitespace collapsing
template<class InputIterator> void appendCollapsingWhitespace(std::wstring* appendTo, InputIterator begin, InputIterator end) {
	appendTo->reserve(appendTo->length() + (end - begin));
	bool lastCharWasWhitespace = (appendTo->empty() ? TRUE : isWhitespace(appendTo->back()));
	while (begin != end) {
		bool thisCharIsWhitespace = isWhitespace(*begin);
		if (!lastCharWasWhitespace || !thisCharIsWhitespace) {
			appendTo->push_back((thisCharIsWhitespace ? L' ' : *begin));
		}
		lastCharWasWhitespace = thisCharIsWhitespace;
		++begin;
	}
}


hubbub_error AppendTokenContentsToWString(const hubbub_token *token, void *pw) {
	std::wstring* out = (std::wstring*) pw;

	switch (token->type) {
		case HUBBUB_TOKEN_DOCTYPE: break;
		case HUBBUB_TOKEN_START_TAG: {
			std::string name(hubbubstr2stdstr(token->data.tag.name));

			if (token->data.tag.ns == HUBBUB_NS_HTML && (name.compare("br") == 0)) {
				out->append(L"\n");
			}
			if (token->data.tag.ns == HUBBUB_NS_HTML && (name.compare("img") == 0)) {
				hubbub_string alttext;
				alttext.len = 0;
				alttext.ptr = (uint8_t*) "";

				// find the image's alt text, then display the alt text
				for (uint32_t i = 0; i < token->data.tag.n_attributes; i++) {
					if (hubbubstr2stdstr(token->data.tag.attributes[i].name).compare("alt") == 0) {
						alttext = token->data.tag.attributes[i].value;
					}
				}

				int wstringBufferLen = MultiByteToWideChar(CP_UTF8, 0, (char*)alttext.ptr, alttext.len, NULL, 0);
				LPWSTR wstr = new wchar_t[wstringBufferLen];
				int wstringLen = MultiByteToWideChar(CP_UTF8, 0, (char*)alttext.ptr, alttext.len, wstr, wstringBufferLen);

				appendCollapsingWhitespace(out, wstr, wstr + wstringLen);
				delete[] wstr;
			}
		} break;
		case HUBBUB_TOKEN_END_TAG: {
			std::string name(hubbubstr2stdstr(token->data.tag.name));

			if (token->data.tag.ns == HUBBUB_NS_HTML && (name.compare("p") == 0 || name.compare("ol") == 0 || name.compare("ul") == 0 || name.compare("h1") == 0 || name.compare("h2") == 0 || name.compare("table") == 0)) {
				out->append(L"\n\n");
			}
			if (token->data.tag.ns == HUBBUB_NS_HTML && (name.compare("tr") == 0 || name.compare("li") == 0)) {
				out->append(L"\n");
			}

		} break;
		case HUBBUB_TOKEN_COMMENT: break;
		case HUBBUB_TOKEN_CHARACTER: {
			int wstringBufferLen = MultiByteToWideChar(CP_UTF8, 0, (char*) token->data.character.ptr, token->data.character.len, NULL,  0);
			LPWSTR wstr = new wchar_t[wstringBufferLen];
			int wstringLen = MultiByteToWideChar(CP_UTF8, 0, (char*)token->data.character.ptr, token->data.character.len, wstr, wstringBufferLen);

			appendCollapsingWhitespace(out, wstr, wstr + wstringLen);
			delete[] wstr;
		} break;
		case HUBBUB_TOKEN_EOF: break;
	}

	return HUBBUB_OK;
}

hubbub_error FindFirstImgHrefToWString(const hubbub_token *token, void *pw) {
	std::wstring* out = (std::wstring*) pw;

	switch (token->type) {
		case HUBBUB_TOKEN_DOCTYPE: break;
		case HUBBUB_TOKEN_START_TAG: {
			if (0 == out->size()) {
				std::string name(hubbubstr2stdstr(token->data.tag.name));

				if (token->data.tag.ns == HUBBUB_NS_HTML && (name.compare("img") == 0)) {
					hubbub_string href;
					href.len = 0;
					href.ptr = (uint8_t*) "";

					// find the image's alt text, then display the alt text
					for (uint32_t i = 0; i < token->data.tag.n_attributes; i++) {
						if (hubbubstr2stdstr(token->data.tag.attributes[i].name).compare("src") == 0) {
							href = token->data.tag.attributes[i].value;
						}
					}

					int wstringBufferLen = MultiByteToWideChar(CP_UTF8, 0, (char*)href.ptr, href.len, NULL, 0);
					LPWSTR wstr = new wchar_t[wstringBufferLen];
					int wstringLen = MultiByteToWideChar(CP_UTF8, 0, (char*)href.ptr, href.len, wstr, wstringBufferLen);

					appendCollapsingWhitespace(out, wstr, wstr + wstringLen);
					delete[] wstr;
				}
			}
		} break;
		case HUBBUB_TOKEN_END_TAG: break;
		case HUBBUB_TOKEN_COMMENT: break;
		case HUBBUB_TOKEN_CHARACTER:  break;
		case HUBBUB_TOKEN_EOF: break;
	}

	return HUBBUB_OK;
}


FeedItem::FeedItem(IFeedItem* backing) : backing(backing) {}


FeedElement* FeedItem::getParent() const {
	HRESULT error;
	IFeed* result;
	error = backing->get_Parent((IDispatch**)&result);
	if (error) {
		return new ErrorFeedElement(L"Already top level; no parent to cd to");
	} else {
		return new FeedFeed(result);
	}
}

FeedElement* FeedItem::getChild(const wstring name) const {
	return new ErrorFeedElement(L"No subelements of a feed item");
}

FeedElement* FeedItem::clone() const {
	return new FeedItem(this->backing);
}

void FeedItem::printContents(const bool filterUnread, std::wostream& out) const {
	out << L"No contents inside an item";
}

std::vector<wstring> FeedItem::getContents() const {
	return std::vector<wstring>();
}

void FeedItem::printDetails(std::wostream& out) const {
	BSTR str;
	DATE pubDate;
	VARIANT_BOOL isRead;
	IFeedEnclosure* enclosure;
	HRESULT error;
	
	error = backing->get_Title(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << std::endl << str << std::endl << std::endl;
		SysFreeString(str);
	}
	
	error = backing->get_Author(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Author: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_Link(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Url: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_Comments(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Comments: " << str << std::endl;
		SysFreeString(str);
	}

	error = backing->get_IsRead(&isRead);
	if (SUCCEEDED(error) && error != S_FALSE) {
		out << INDENT << "Read: " << (isRead ? "TRUE" : "FALSE") << std::endl;
	}
	
	error = backing->get_Enclosure((IDispatch**) &enclosure);
	if (SUCCEEDED(error) && error != S_FALSE) {
		FEEDS_DOWNLOAD_STATUS dlstatus;
		FEEDS_DOWNLOAD_ERROR dlerror;

		out << INDENT << "Enclosure:" << std::endl;

		error = enclosure->get_Type(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Type: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_Url(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Url: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_DownloadStatus(&dlstatus);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Status: " << downloadStatus2String(dlstatus) << std::endl;
		}

		error = enclosure->get_LocalPath(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Local Path: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_LastDownloadError(&dlerror);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Error: " << downloadError2String(dlerror) << std::endl;
		}

		error = enclosure->get_DownloadMimeType(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Type: " << str << std::endl;
			SysFreeString(str);
		}

		error = enclosure->get_DownloadUrl(&str);
		if (SUCCEEDED(error) && error != S_FALSE) {
			out << INDENT << INDENT << "Download Url: " << str << std::endl;
			SysFreeString(str);
		}


 		enclosure->Release();
 	}
	
	error = backing->get_PubDate(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			out << INDENT << "Published: " << str << std::endl;
			SysFreeString(str);
		} else {
			out << INDENT << "Published: " << "ERROR" << std::endl;
		}
	}
	
	error = backing->get_Modified(&pubDate);
	if (SUCCEEDED(error) && error != S_FALSE) {
		error = VarBstrFromDate(pubDate, GetSystemDefaultLCID(), VAR_FOURDIGITYEARS, &str);
		if (SUCCEEDED(error)) {
			out << INDENT << "Modified: " << str << std::endl;
			SysFreeString(str);
		} else {
			out << INDENT << "Modified: " << "ERROR" << std::endl;
		}
	}

	out << std::endl;
	
	error = backing->get_Description(&str);
	if (SUCCEEDED(error) && error != S_FALSE) {
		
		int utf8StringBufferLen = WideCharToMultiByte(CP_UTF8, 0, str, SysStringLen(str), NULL, 0, NULL, NULL);
		LPSTR utf8String = new char[utf8StringBufferLen];
		int utf8StringLen = WideCharToMultiByte(CP_UTF8, 0, str, SysStringLen(str), utf8String, utf8StringBufferLen, NULL, NULL);

		if (utf8StringLen != 0) {
			wstring prettyOutput = wstring();
			hubbub_parser* parser;
			hubbub_error huberror;
			
			huberror = hubbub_parser_create("UTF-8", false, &parser);
			if (huberror == HUBBUB_OK) {
				hubbub_parser_optparams tokenCallback;
				tokenCallback.token_handler.handler = *AppendTokenContentsToWString;
				tokenCallback.token_handler.pw = &prettyOutput;
				huberror = hubbub_parser_setopt(parser, HUBBUB_PARSER_TOKEN_HANDLER, &tokenCallback);
				huberror = hubbub_parser_parse_chunk(parser, (uint8_t*) utf8String, utf8StringLen);
				huberror = hubbub_parser_completed(parser);
				huberror = hubbub_parser_destroy(parser);
			}

			out << prettyOutput << std::endl << std::endl;
		}

		delete[] utf8String;
		SysFreeString(str);
	}
}

wstring FeedItem::getPath() const {
	IFeed* parent;
	backing->get_Parent((IDispatch**)&parent);
	
	BSTR  parentPath;
	parent->get_Path(&parentPath);
	
	LONG localid;
	backing->get_LocalId(&localid);
	
	wchar_t retVal[STR_BUFFER_SIZE];
	swprintf(retVal, STR_BUFFER_SIZE, L"%ls\\%d", (wchar_t *)parentPath, localid);
	
	parent->Release();
	SysFreeString(parentPath);
	return retVal;
}

bool FeedItem::isError() const { return false; }

std::pair<HRESULT, std::wstring> FeedItem::getAttachmentFile() const {
	FEEDS_DOWNLOAD_STATUS status;
	IFeedEnclosure* enclosure;
	HRESULT result;
	
	result = backing->get_Enclosure((IDispatch**)&enclosure);
	if (SUCCEEDED(result) && result != S_FALSE) {
		result = enclosure->get_DownloadStatus(&status);
		if (SUCCEEDED(result) && result != S_FALSE) {
			BSTR bpath;

			if (FDS_DOWNLOADED == status) {
				result = enclosure->get_LocalPath(&bpath);
			} else {
				result = enclosure->get_Url(&bpath);
			}

			if (SUCCEEDED(result) && result != S_FALSE) {
				wchar_t wpath[STR_BUFFER_SIZE];
				swprintf(wpath, STR_BUFFER_SIZE, L"%s", bpath);
				SysFreeString(bpath);
				return std::pair<HRESULT, std::wstring>(S_OK, wpath);
			} else {
				return std::pair<HRESULT, std::wstring>(result, L"");
			}

		} else {
			return std::pair<HRESULT, std::wstring>(result, L"");
		}
	} else {
		return std::pair<HRESULT, std::wstring>(result, L"");
	}
}

std::pair<HRESULT, std::wstring> FeedItem::getUrl() const {
	HRESULT result;
	BSTR bpath;

	result = backing->get_Link(&bpath);
	if (SUCCEEDED(result) && result != S_FALSE) {
		wchar_t wpath[STR_BUFFER_SIZE];
		swprintf(wpath, STR_BUFFER_SIZE, L"%s", bpath);
		SysFreeString(bpath);
		return std::pair<HRESULT, std::wstring>(S_OK, wpath);
	}
	else {
		return std::pair<HRESULT, std::wstring>(result, L"");
	}
}

HRESULT FeedItem::markAsRead() {
	return backing->put_IsRead(VARIANT_TRUE);
}

HRESULT FeedItem::attachImageFromDescription() {
	HRESULT error;
	BSTR description;

	error = backing->get_Description(&description);
	if (SUCCEEDED(error) && error != S_FALSE) {

		int utf8StringBufferLen = WideCharToMultiByte(CP_UTF8, 0, description, SysStringLen(description), NULL, 0, NULL, NULL);
		LPSTR utf8String = new char[utf8StringBufferLen];
		int utf8StringLen = WideCharToMultiByte(CP_UTF8, 0, description, SysStringLen(description), utf8String, utf8StringBufferLen, NULL, NULL);

		if (0 == utf8StringLen) {
			error = S_FALSE;
		} else {
			wstring imgHref;
			hubbub_parser* parser;
			hubbub_error huberror;

			huberror = hubbub_parser_create("UTF-8", false, &parser);
			if (huberror == HUBBUB_OK) {
				hubbub_parser_optparams tokenCallback;
				tokenCallback.token_handler.handler = *FindFirstImgHrefToWString;
				tokenCallback.token_handler.pw = &imgHref;
				huberror = hubbub_parser_setopt(parser, HUBBUB_PARSER_TOKEN_HANDLER, &tokenCallback);
				huberror = hubbub_parser_parse_chunk(parser, (uint8_t*)utf8String, utf8StringLen);
				huberror = hubbub_parser_completed(parser);
				huberror = hubbub_parser_destroy(parser);
			}

			if (huberror != HUBBUB_OK) {
				error = E_FAIL;
			} else if (0 == imgHref.size()) {
				error = S_FALSE;
			} else {
				IFeedEnclosure* enclosure;
				error = backing->get_Enclosure((IDispatch**) &enclosure);

				if (S_FALSE == error) {
					BSTR xml;
					error = backing->Xml(FXIF_CF_EXTENSIONS, &xml);
					
					if (SUCCEEDED(error)) {
						IFeed* feed;
						error = backing->get_Parent((IDispatch**)& feed);

						if (SUCCEEDED(error)) {
							wstring toMerge(L"<rss version=\"2.0\"><channel><item>");
							toMerge.append(L"<enclosure url=\"");
							toMerge.append(imgHref);
							toMerge.append(L"\" />");
							toMerge.append(xml + wcslen(L"<item>"));
							toMerge.append(L"</channel></rss>");

							BSTR securityUrl;
							backing->get_DownloadUrl(&securityUrl);

							BSTR toMerge2 = SysAllocString(toMerge.c_str());
							BSTR toMerge3;

							IFeedsManager* manager;

							CoCreateInstance(
								CLSID_FeedsManager, NULL, CLSCTX_ALL, IID_IFeedsManager,
								(void**)&manager);

							manager->Normalize(toMerge2, &toMerge3);
							manager->Release();

							error = feed->Merge(toMerge3, securityUrl);
						}
					}

				} else if (SUCCEEDED(error)) {
					enclosure->Release();
					error = S_FALSE;
				}
			}
		}

		delete[] utf8String;
		SysFreeString(description);
	}

	return error;
}

HRESULT FeedItem::downloadAttachmentAsync() {
	FEEDS_DOWNLOAD_STATUS status;
	IFeedEnclosure* enclosure;
	HRESULT result;
	
	result = backing->get_Enclosure((IDispatch**)&enclosure);
	if (SUCCEEDED(result) && result != S_FALSE) {
		result = enclosure->AsyncDownload();
	}
	return result;
}

