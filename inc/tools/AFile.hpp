#ifndef AFILE_HPP
#define AFILE_HPP 1

class AFile: public ILoader, public IValidator
{

	private:

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
		virtual EValidator validate() const __attribute__((warn_unused_result));

	private:

		AFile();
		AFile(const AFile &);
		AFile & operator=(const AFile &);
		virtual void closeFile();
		virtual bool openFile() __attribute__((warn_unused_result));

};

#endif // AFILE_HPP
