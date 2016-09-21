#ifndef BLUNTFEEDITEMS_H
#define BLUNTFEEDITEMS_H

#include <msfeeds.h>
#include <vector>


namespace Tests
{

	class NotimplFeedItem : public IFeedItem {
	 public:
		NotimplFeedItem();

		virtual HRESULT STDMETHODCALLTYPE Xml(FEEDS_XML_INCLUDE_FLAGS includeFlags, BSTR *xml);
		virtual HRESULT STDMETHODCALLTYPE get_Title(BSTR *title);
		virtual HRESULT STDMETHODCALLTYPE get_Link(BSTR *linkUrl);
		virtual HRESULT STDMETHODCALLTYPE get_Guid(BSTR *itemGuid);
		virtual HRESULT STDMETHODCALLTYPE get_Description(BSTR *description);
		virtual HRESULT STDMETHODCALLTYPE get_PubDate(DATE *pubDate);
		virtual HRESULT STDMETHODCALLTYPE get_Comments(BSTR *comments);
		virtual HRESULT STDMETHODCALLTYPE get_Author(BSTR *author);
		virtual HRESULT STDMETHODCALLTYPE get_Enclosure(IDispatch **disp);
		virtual HRESULT STDMETHODCALLTYPE get_IsRead(VARIANT_BOOL *isRead);
		virtual HRESULT STDMETHODCALLTYPE put_IsRead(VARIANT_BOOL isRead);
		virtual HRESULT STDMETHODCALLTYPE get_LocalId(LONG *itemId);
		virtual HRESULT STDMETHODCALLTYPE get_Parent(IDispatch **disp);
		virtual HRESULT STDMETHODCALLTYPE Delete(void);
		virtual HRESULT STDMETHODCALLTYPE get_DownloadUrl(BSTR *itemUrl);
		virtual HRESULT STDMETHODCALLTYPE get_LastDownloadTime(DATE *lastDownload);
		virtual HRESULT STDMETHODCALLTYPE get_Modified(DATE *modified);

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &, void **);
		virtual ULONG STDMETHODCALLTYPE AddRef(void);
		virtual ULONG STDMETHODCALLTYPE Release(void);
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *);
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo **);
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *);
		virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *);
	 private:
		unsigned int refs;
	};
	
	class NotimplFeedFeed : public IFeed {
	 public:
		NotimplFeedFeed();
		
		virtual HRESULT STDMETHODCALLTYPE get_Title(BSTR *title);
		virtual HRESULT STDMETHODCALLTYPE get_DownloadUrl(BSTR *feedUrl);
		virtual HRESULT STDMETHODCALLTYPE get_Url(BSTR *feedUrl);
		virtual HRESULT STDMETHODCALLTYPE GetItem(LONG itemId, IDispatch **disp);
		virtual HRESULT STDMETHODCALLTYPE Xml(LONG count, FEEDS_XML_SORT_PROPERTY sortProperty, FEEDS_XML_SORT_ORDER sortOrder, FEEDS_XML_FILTER_FLAGS filterFlags, FEEDS_XML_INCLUDE_FLAGS includeFlags, BSTR *xml);
		virtual HRESULT STDMETHODCALLTYPE get_Name(BSTR *name);
		virtual HRESULT STDMETHODCALLTYPE Rename(BSTR name);
		virtual HRESULT STDMETHODCALLTYPE put_Url(BSTR feedUrl);
		virtual HRESULT STDMETHODCALLTYPE get_LocalId(BSTR *feedGuid);
		virtual HRESULT STDMETHODCALLTYPE get_Path(BSTR *path);
		virtual HRESULT STDMETHODCALLTYPE Move(BSTR newParentPath);
		virtual HRESULT STDMETHODCALLTYPE get_Parent(IDispatch **disp);
		virtual HRESULT STDMETHODCALLTYPE get_LastWriteTime(DATE *lastWrite);
		virtual HRESULT STDMETHODCALLTYPE Delete(void);
		virtual HRESULT STDMETHODCALLTYPE Download(void);
		virtual HRESULT STDMETHODCALLTYPE AsyncDownload(void);
		virtual HRESULT STDMETHODCALLTYPE CancelAsyncDownload(void);
		virtual HRESULT STDMETHODCALLTYPE get_SyncSetting(FEEDS_SYNC_SETTING *syncSetting);
		virtual HRESULT STDMETHODCALLTYPE put_SyncSetting(FEEDS_SYNC_SETTING syncSetting);
		virtual HRESULT STDMETHODCALLTYPE get_Interval(LONG *minutes);
		virtual HRESULT STDMETHODCALLTYPE put_Interval(LONG minutes);
		virtual HRESULT STDMETHODCALLTYPE get_LastDownloadTime(DATE *lastDownload);
		virtual HRESULT STDMETHODCALLTYPE get_LocalEnclosurePath(BSTR *path);
		virtual HRESULT STDMETHODCALLTYPE get_Items(IDispatch **disp);
		virtual HRESULT STDMETHODCALLTYPE get_Description(BSTR *description);
		virtual HRESULT STDMETHODCALLTYPE get_Link(BSTR *homePage);
		virtual HRESULT STDMETHODCALLTYPE get_Image(BSTR *imageUrl);
		virtual HRESULT STDMETHODCALLTYPE get_LastBuildDate(DATE *lastBuildDate);
		virtual HRESULT STDMETHODCALLTYPE get_PubDate(DATE *lastPopulateDate);
		virtual HRESULT STDMETHODCALLTYPE get_Ttl(LONG *ttl);
		virtual HRESULT STDMETHODCALLTYPE get_Language(BSTR *language);
		virtual HRESULT STDMETHODCALLTYPE get_Copyright(BSTR *copyright);
		virtual HRESULT STDMETHODCALLTYPE get_MaxItemCount(LONG *count);
		virtual HRESULT STDMETHODCALLTYPE put_MaxItemCount(LONG count);
		virtual HRESULT STDMETHODCALLTYPE get_DownloadEnclosuresAutomatically(VARIANT_BOOL *downloadEnclosuresAutomatically);
		virtual HRESULT STDMETHODCALLTYPE put_DownloadEnclosuresAutomatically(VARIANT_BOOL downloadEnclosuresAutomatically);
		virtual HRESULT STDMETHODCALLTYPE get_DownloadStatus(FEEDS_DOWNLOAD_STATUS *status);
		virtual HRESULT STDMETHODCALLTYPE get_LastDownloadError(FEEDS_DOWNLOAD_ERROR *error);
		virtual HRESULT STDMETHODCALLTYPE Merge(BSTR feedXml, BSTR feedUrl);
		virtual HRESULT STDMETHODCALLTYPE get_IsList(VARIANT_BOOL *isList);
		virtual HRESULT STDMETHODCALLTYPE MarkAllItemsRead(void);
		virtual HRESULT STDMETHODCALLTYPE GetWatcher(FEEDS_EVENTS_SCOPE scope, FEEDS_EVENTS_MASK mask, IDispatch **disp);
		virtual HRESULT STDMETHODCALLTYPE get_UnreadItemCount(LONG *count);
		virtual HRESULT STDMETHODCALLTYPE get_ItemCount(LONG *count);

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &, void **);
		virtual ULONG STDMETHODCALLTYPE AddRef(void);
		virtual ULONG STDMETHODCALLTYPE Release(void);
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *);
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo **);
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *);
		virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *);
	 private:
		unsigned int refs;
	};

	class ChildrenFeedFeed : public NotimplFeedFeed {
	 public:
		ChildrenFeedFeed(std::vector<IFeedItem*> items);
		virtual HRESULT STDMETHODCALLTYPE get_ItemCount(LONG *count);
		virtual HRESULT STDMETHODCALLTYPE GetItem(LONG itemId, IDispatch **disp);
		virtual HRESULT STDMETHODCALLTYPE get_Items(IDispatch **disp);
	 private:
		std::vector<IFeedItem*> items;
	};
	
	class VectorFeedsEnum : public IFeedsEnum {
	 public:
		VectorFeedsEnum(std::vector<IFeedItem*> items);
		virtual HRESULT STDMETHODCALLTYPE get_Count(LONG *count);
		virtual HRESULT STDMETHODCALLTYPE Item(LONG index,IDispatch **disp);
		
		// THE DOCS NEVER MENTION THIS METHOD
		virtual HRESULT STDMETHODCALLTYPE VectorFeedsEnum::get__NewEnum(IEnumVARIANT **enumVar);

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &, void **);
		virtual ULONG STDMETHODCALLTYPE AddRef(void);
		virtual ULONG STDMETHODCALLTYPE Release(void);
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *);
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo **);
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *);
		virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *);
	 private:
		std::vector<IFeedItem*> items;
		unsigned int refs;
	};
	
}



#define FeedItemBstr(prop)                                                         \
	class prop##FeedItem : public Tests::NotimplFeedItem {                         \
	 public:                                                                       \
		prop##FeedItem(wstring value) : value(value), err(S_OK) {}                 \
		prop##FeedItem(wstring value, HRESULT err) : value(value), err(err) {}     \
		virtual HRESULT STDMETHODCALLTYPE get_##prop(BSTR* out) {                  \
			*out = SysAllocString(this->value.c_str()); return err;                \
		}                                                                          \
	 private:                                                                      \
		wstring value;                                                             \
		HRESULT err;                                                               \
	}

#define FeedItemLong(prop)                                                         \
	class prop##FeedItem : public Tests::NotimplFeedItem {                         \
	 public:                                                                       \
		prop##FeedItem(LONG value) : value(value), err(S_OK) {}                    \
		prop##FeedItem(LONG value, HRESULT err) : value(value), err(err) {}        \
		virtual HRESULT STDMETHODCALLTYPE get_##prop(LONG* out) {                  \
			*out = value; return err;                                              \
		}                                                                          \
	 private:                                                                      \
		LONG value;                                                                \
		HRESULT err;                                                               \
	}


#define FeedFeedBstr(prop)                                                         \
	class prop##FeedFeed : public Tests::NotimplFeedFeed {                         \
	 public:                                                                       \
		prop##FeedFeed(wstring value) : value(value), err(S_OK) {}                 \
		prop##FeedFeed(wstring value, HRESULT err) : value(value), err(err) {}     \
		virtual HRESULT STDMETHODCALLTYPE get_##prop(BSTR* out) {                  \
			*out = SysAllocString(this->value.c_str()); return err;                \
		}                                                                          \
	 private:                                                                      \
		wstring value;                                                             \
		HRESULT err;                                                               \
	}




#endif        //  #ifndef BLUNTFEEDITEMS_H
