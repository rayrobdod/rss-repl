#include "CppUnitTest.h"
// I can't get the build to find the main project's methods from the test project
#include "../../main/cpp/FeedElement_Folder.cpp"
#include "../../main/cpp/FeedElement_Null.cpp"
#include "../../main/cpp/FeedElement_Feed.cpp"
#include "../../main/cpp/FeedElement_Item.cpp"
#include "../../main/cpp/FeedElement_shared.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	class BluntFeedItem : public IFeedItem {
	public:
		BluntFeedItem() : title(L""), url(L""), urlRetVal(S_FALSE) {}

		virtual HRESULT STDMETHODCALLTYPE Xml(FEEDS_XML_INCLUDE_FLAGS includeFlags, __RPC__deref_out_opt BSTR *xml) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Title(__RPC__deref_out_opt BSTR *title) {
			*title = SysAllocString(this->title.c_str()); return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE get_Link(__RPC__deref_out_opt BSTR *linkUrl) {
			*linkUrl = SysAllocString(this->url.c_str()); return urlRetVal;
		}
		virtual HRESULT STDMETHODCALLTYPE get_Guid(__RPC__deref_out_opt BSTR *itemGuid) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Description(__RPC__deref_out_opt BSTR *description) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_PubDate(__RPC__out DATE *pubDate) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Comments(__RPC__deref_out_opt BSTR *comments) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Author(__RPC__deref_out_opt BSTR *author) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Enclosure(__RPC__deref_out_opt IDispatch **disp) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_IsRead(__RPC__out VARIANT_BOOL *isRead) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE put_IsRead(VARIANT_BOOL isRead) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_LocalId(__RPC__out LONG *itemId) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Parent(__RPC__deref_out_opt IDispatch **disp) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE Delete(void) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_DownloadUrl(__RPC__deref_out_opt BSTR *itemUrl) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_LastDownloadTime(__RPC__out DATE *lastDownload) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE get_Modified(__RPC__out DATE *modified) { return E_NOTIMPL; }

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &, void **) { return E_NOTIMPL; }
		virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }
		virtual ULONG STDMETHODCALLTYPE Release(void) { return 0; }
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo **) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *) { return E_NOTIMPL; }
		virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) { return E_NOTIMPL; }
	public:
		wstring title;
		wstring url;
		HRESULT urlRetVal;
	};

	TEST_CLASS(Commands)
	{
	public:

		TEST_METHOD(FeedItem_DetailsContainsTitle) {
			BluntFeedItem backing;
			backing.title = L"The Title";
			::FeedItem dut(& backing);
			Assert::AreNotEqual(wstring::npos, dut.getDetailsString().find(L"The Title"));
		}

		TEST_METHOD(FeedItem_DetailsNotContainsUrlWhenSfalse) {
			BluntFeedItem backing;
			backing.urlRetVal = S_FALSE;
			backing.url = L"Theurl";
			::FeedItem dut(& backing);
			Assert::AreEqual(wstring::npos, dut.getDetailsString().find(L"Theurl"));
		}

		TEST_METHOD(FeedItem_DetailsNotContainsUrlWhenSok) {
			BluntFeedItem backing;
			backing.urlRetVal = S_OK;
			backing.url = L"Theurl";
			::FeedItem dut(& backing);
			Assert::AreNotEqual(wstring::npos, dut.getDetailsString().find(L"Theurl"));
		}

		TEST_METHOD(FeedItem_IsErrorIsFalse) {
			BluntFeedItem backing;
			::FeedItem dut(& backing);
			Assert::IsFalse(dut.isError());
		}

		TEST_METHOD(FeedItem_ContentsIsEmpty) {
			std::vector<std::wstring> empty;
			BluntFeedItem backing;
			::FeedItem dut(& backing);
			Assert::IsTrue(empty == dut.getContents());
		}

		TEST_METHOD(FeedItem_ContentsStringIsEmpty) {
			BluntFeedItem backing;
			::FeedItem dut(& backing);
			Assert::AreEqual(wstring::npos, dut.getContentsString(false).find(L"\n"));
		}

		TEST_METHOD(FeedItem_ChildIsError) {
			BluntFeedItem backing;
			::FeedItem dut(& backing);
			::FeedElement* child = dut.followPath(L"asdasd");
			Assert::IsTrue(child->isError());
			delete child;
		}

		TEST_METHOD(FeedItem_GetUrlWhenError) {
			BluntFeedItem backing;
			backing.urlRetVal = E_NOTIMPL;
			backing.url = L"Theurl";
			::FeedItem dut(& backing);
			std::pair<HRESULT, std::wstring> exp(E_NOTIMPL, L"");
			Assert::IsTrue(exp == dut.getUrl());
		}

		TEST_METHOD(FeedItem_GetUrlWhenOk) {
			BluntFeedItem backing;
			backing.urlRetVal = S_OK;
			backing.url = L"Theurl";
			::FeedItem dut(& backing);
			std::pair<HRESULT, std::wstring> exp(S_OK, L"Theurl");
			Assert::IsTrue(exp == dut.getUrl());
		}
	};
}
