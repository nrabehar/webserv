#ifndef AFILE_HPP
#define AFILE_HPP 1

class AFile: public ILoader, public IChecker
{

	protected:

		std::string _name;
		std::ifstream _stream;
		std::string _content;

	public:

		AFile(const std::string &);
		virtual ~AFile();
		virtual const std::ifstream & getStream() const;
		virtual const std::string & getName() const;
		virtual const std::string & getContent() const;
		virtual bool load() __attribute__((warn_unused_result));
		virtual EFileStatus check() const __attribute__((warn_unused_result));
		virtual bool openFile() __attribute__((warn_unused_result));

	private:

		AFile();
		AFile(const AFile &);
		AFile & operator=(const AFile &);
		virtual void closeFile();

};

#endif // AFILE_HPP
