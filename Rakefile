CURRENT_ENV = ENV['os']

namespace "ci" do
	desc "Continous Integration Build Task"
	task :build do
		puts "Starting CI Build on #{CURRENT_ENV}."
		sh "codeblocks --rebuild *.workspace --target=\"Release\""
	end
end